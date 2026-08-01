#include "Character/MyCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Data/TagToComboTable.h"
#include "Actor/WeaponActor.h"
#include "Engine/AssetManager.h"
#include "Game/MyGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Inventory/InventoryComponent.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerState.h"
#include "UI/MyHUD.h"
#include "Inventory/Data/ConsumableDataAsset.h"
#include "Inventory/Data/WeaponDataAsset.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "Game/MyGameModeBase.h"
#include "SaveGame/MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/MySaveSubsystem.h"

class UMySaveSubsystem;

AMyCharacter::AMyCharacter()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Initialize Ability Actor Info for server
	InitAbilityActorInfo();
	LoadFromSaveData();
}

void AMyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Initialize Ability Actor Info for client
	InitAbilityActorInfo();
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMyCharacter, EquippedWeaponGuid)
}

void AMyCharacter::Destroyed()
{
	if (const AMyGameModeBase* MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		MyGameMode->OnPlayerDied();	
	}
	
	Super::Destroyed();
}

void AMyCharacter::Die()
{
	Super::Die();
	
	if (SpawnedWeaponActor)
	{
		SpawnedWeaponActor->SetLifeSpan(DeathLifeSpan);
	}
	
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(MyGameplayTags::Ability_Passive_Common_Death);
	AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
	SetLifeSpan(DeathLifeSpan);
}

void AMyCharacter::AddToLevel(int32 InLevel)
{
	AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	if (!MyPlayerState) return;
	MyPlayerState->AddToLevel(InLevel);
}

void AMyCharacter::AddToXP(int32 InXP)
{
	AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	if (!MyPlayerState) return;
	MyPlayerState->AddToXP(InXP);
}

void AMyCharacter::AddToAttributePoints(int32 InAttributePoints)
{
	AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	if (!MyPlayerState) return;
	MyPlayerState->AddToAttributePoints(InAttributePoints);
}

int32 AMyCharacter::GetPlayerLevel()
{
	const AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	check(MyPlayerState)
	return MyPlayerState->GetPlayerLevel();	
}

int32 AMyCharacter::GetXP()
{
	const AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	check(MyPlayerState)
	return MyPlayerState->GetXP();
}

int32 AMyCharacter::GetAttributePoints()
{
	const AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	check(MyPlayerState)
	return MyPlayerState->GetAttributePoints();
}

int32 AMyCharacter::FindLevelByXP(int32 XP)
{
	const AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	check(MyPlayerState)
	return MyPlayerState->LevelUpInfo->FindLevelByXP(XP);
}

int32 AMyCharacter::GetAttributePointsRewardFromInfo(int32 Level)
{
	const AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(GetPlayerState());
	check(MyPlayerState)
	return MyPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

void AMyCharacter::EquipWeapon(const FGuid InItemGuid)
{
	ServerEquipWeapon(InItemGuid);
}

void AMyCharacter::UnequipWeapon()
{
	ServerUnequipWeapon();
}

void AMyCharacter::UseConsumable(FGuid InItemGuid)
{
	ServerUseConsumable(InItemGuid);
}

void AMyCharacter::AddItemToInventory(FPrimaryAssetId ItemAssetId, int32 Amount)
{
	if (!HasAuthority()) return;
	if (!ItemAssetId.IsValid()) return;
	
	InventoryComponent->AddItemByPrimaryAssetId(ItemAssetId, Amount);
}

void AMyCharacter::ServerEquipWeapon_Implementation(FGuid InItemGuid)
{
	// Unequip weapon if already equipped one.
	if (EquippedWeaponGuid.IsValid())
	{
		ServerUnequipWeapon();
	}
	
	// Trigger OnRep_EquippedWeaponGuid
	EquippedWeaponGuid = InItemGuid;
	
	// Server Side
	TWeakObjectPtr<AMyCharacter> WeakThis = this;
	
	// Bind callback to OnLoadedDelegate, spawn and attach Weapon when Weapon Asset loaded
	FOnAssetLoadedDelegate OnLoadedDelegate;
	OnLoadedDelegate.BindLambda([WeakThis](UPrimaryDataAsset* LoadedAsset)
	{
		AMyCharacter* StrongThis = WeakThis.Get();
		if (!StrongThis) return;
		
		const UWeaponDataAsset* LoadedWeaponDataAsset = Cast<UWeaponDataAsset>(LoadedAsset);
		if (!LoadedWeaponDataAsset) return;
		
		// Get AssetID and the "Latest" Index, prevent Inventory changes during async loading
		int32 CurrentIndex = -1;
		FPrimaryAssetId CurrentAssetId;
		StrongThis->InventoryComponent->GetAssetIdAndIndexByGuid(StrongThis->EquippedWeaponGuid, CurrentIndex, CurrentAssetId);
		
		if (CurrentIndex != -1)
		{
			StrongThis->AttachWeaponAndSetData(CurrentIndex, LoadedWeaponDataAsset);
		}
	});
	
	int32 AssetIndex = -1;
	FPrimaryAssetId AssetId;
	InventoryComponent->GetAssetIdAndIndexByGuid(EquippedWeaponGuid, AssetIndex, AssetId);
	
	// Get Asset
	InventoryComponent->GetPrimaryAssetAsync(AssetId, OnLoadedDelegate);

}

void AMyCharacter::AttachWeaponAndSetData(const int32 EquippedWeaponIndex, const UWeaponDataAsset* LoadedWeaponAsset)
{
	SpawnAndAttachWeapon(LoadedWeaponAsset);
			
	// Update server's ASC ComboDataAsset and WeaponSockets
	UpdateCombatDataFromWeapon(LoadedWeaponAsset);
	
	if (HasAuthority())
	{
		InventoryComponent->SetEntryEquipped(EquippedWeaponIndex, true);	
	}
}

void AMyCharacter::SpawnAndAttachWeapon(const UWeaponDataAsset* LoadedWeaponAsset)
{
	TSoftClassPtr<AWeaponActor> WeaponActorSoftClassPtr = LoadedWeaponAsset->WeaponActorClass;
	
	if (WeaponActorSoftClassPtr.IsNull()) return;
	
	// Already in memory
	if (WeaponActorSoftClassPtr.IsValid())
	{
		OnWeaponClassLoaded(WeaponActorSoftClassPtr);
		return;
	}
	
	// Async load Weapon Class 
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	
	FSoftObjectPath WeaponPath = WeaponActorSoftClassPtr.ToSoftObjectPath();
	
	// CreateUObject has a weak reference mechanism, If "this" no longer exists, Unreal will automatically cancel or not execute the call function.
	StreamableManager.RequestAsyncLoad(
		WeaponPath,
		FStreamableDelegate::CreateUObject(this, &AMyCharacter::OnWeaponClassLoaded, WeaponActorSoftClassPtr)
	);
}

void AMyCharacter::OnWeaponClassLoaded(TSoftClassPtr<AWeaponActor> LoadedSoftClassPtr)
{
	UClass* WeaponClass = LoadedSoftClassPtr.Get();
	if (!WeaponClass) return;
		
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	
	SpawnedWeaponActor = GetWorld()->SpawnActor<AWeaponActor>(WeaponClass, GetActorLocation(), GetActorRotation(), SpawnParameters);
	if (!SpawnedWeaponActor) return;	
	
	SpawnedWeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
}

void AMyCharacter::UpdateCombatDataFromWeapon(const UWeaponDataAsset* LoadedWeaponAsset)
{
	UMyAbilitySystemComponent* MyASC = Cast<UMyAbilitySystemComponent>(AbilitySystemComponent);
	check(MyASC)
	
	if (LoadedWeaponAsset)
	{
		// Set Combo Table to corresponding weapon type
		MyASC->SetComboDataAsset(TagToComboTable->GetComboTableByTag(LoadedWeaponAsset->WeaponTypeTag));
		
		// Set Weapon Socket for GA tracing
		EquippedWeaponSocketNames = LoadedWeaponAsset->WeaponSockets;
		
		// Set Weapon Type Tag
		EquippedWeaponTypeTag = LoadedWeaponAsset->WeaponTypeTag;
		
		// Give Weapon Ability
		if (LoadedWeaponAsset->AbilityClass)
		{
			const FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(LoadedWeaponAsset->AbilityClass);
			if (HasAuthority())
			{
				AbilitySystemComponent->GiveAbility(AbilitySpec);
				EquippedWeaponAbilitySpecHandle = AbilitySpec.Handle;
			} 
			EquippedWeaponAbilityTag = LoadedWeaponAsset->AbilityTag;	
		}
		
		// Apply Weapon Stats Effect
		if (LoadedWeaponAsset->EffectClass && HasAuthority())
		{
			const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
			const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(LoadedWeaponAsset->EffectClass, 1, ContextHandle);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, MyGameplayTags::SetByCaller_Weapon_Attack, LoadedWeaponAsset->Attack);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, MyGameplayTags::SetByCaller_Weapon_ElementalAttack, LoadedWeaponAsset->ElementalAttack);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, MyGameplayTags::SetByCaller_Weapon_CriticalChance, LoadedWeaponAsset->CriticalChance);
			EquippedWeaponActivateEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
	// Clear ComboDataAsset, clear WeaponSockets, ClearAbility and RemoveEffect if Asset is nullptr.
	else
	{
		SpawnedWeaponActor = nullptr;
		
		MyASC->SetComboDataAsset(nullptr);
		EquippedWeaponSocketNames.Empty();
		EquippedWeaponTypeTag = FGameplayTag();
		
		// Remove Weapon Ability
		if (HasAuthority() && EquippedWeaponAbilitySpecHandle.IsValid())
		{
			AbilitySystemComponent->ClearAbility(EquippedWeaponAbilitySpecHandle);
			EquippedWeaponAbilitySpecHandle = FGameplayAbilitySpecHandle();
		} 
		EquippedWeaponAbilityTag = FGameplayTag();
		
		// Remove Weapon Effect
		if (HasAuthority() && EquippedWeaponActivateEffectHandle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(EquippedWeaponActivateEffectHandle, -1);
			EquippedWeaponActivateEffectHandle = FActiveGameplayEffectHandle();
		}
	}
}

void AMyCharacter::ServerUnequipWeapon_Implementation()
{
	if (!EquippedWeaponGuid.IsValid()) return;
	
	// Get Index, then update entry's bIsEquipped
	int32 EquippedWeaponIndex = -1;
	FPrimaryAssetId ItemAssetId;
	
	InventoryComponent->GetAssetIdAndIndexByGuid(EquippedWeaponGuid, EquippedWeaponIndex, ItemAssetId);
	InventoryComponent->SetEntryEquipped(EquippedWeaponIndex, false);
	
	// Sever destroy Weapon Actor
	if (SpawnedWeaponActor)
	{
		SpawnedWeaponActor->Destroy();
	}
	// Sever clear ComboTable, WeaponSockets, WeaponAbility, WeaponEffect
	UpdateCombatDataFromWeapon(nullptr);
	
	// Trigger OnRep_EquippedWeaponGuid
	EquippedWeaponGuid = FGuid();
}

void AMyCharacter::OnRep_EquippedWeaponGuid()
{
	// Client destroy Weapon Actor
	if (SpawnedWeaponActor)
	{
		SpawnedWeaponActor->Destroy();	
	}
	
	if (EquippedWeaponGuid.IsValid())
	{
		TWeakObjectPtr<AMyCharacter> WeakThis = this;
	
		// Bind callback to OnLoadedDelegate, spawn and attach Weapon when Weapon Asset loaded
		FOnAssetLoadedDelegate OnLoadedDelegate;
		OnLoadedDelegate.BindLambda([WeakThis](UPrimaryDataAsset* LoadedAsset)
		{
			AMyCharacter* StrongThis = WeakThis.Get();
			if (!StrongThis) return;
		
			const UWeaponDataAsset* LoadedWeaponDataAsset = Cast<UWeaponDataAsset>(LoadedAsset);
			if (!LoadedWeaponDataAsset) return;
		
			// Get AssetID and the "Latest" Index, prevent Inventory changes during async loading
			int32 CurrentIndex = -1;
			FPrimaryAssetId CurrentAssetId;
			StrongThis->InventoryComponent->GetAssetIdAndIndexByGuid(StrongThis->EquippedWeaponGuid, CurrentIndex, CurrentAssetId);
		
			if (CurrentIndex != -1)
			{
				StrongThis->AttachWeaponAndSetData(CurrentIndex, LoadedWeaponDataAsset);
			}
		});
	
		int32 AssetIndex = -1;
		FPrimaryAssetId AssetId;
		InventoryComponent->GetAssetIdAndIndexByGuid(EquippedWeaponGuid, AssetIndex, AssetId);
	
		// Get Asset
		InventoryComponent->GetPrimaryAssetAsync(AssetId, OnLoadedDelegate);
	}
	else
	{
		// Client destroy WeaponActor
		if (SpawnedWeaponActor)
		{
			SpawnedWeaponActor->Destroy();	
		}
		
		// Client side clear combat data (don't clear ability and effect)
		UpdateCombatDataFromWeapon(nullptr);
	}
}

void AMyCharacter::ServerUseConsumable_Implementation(const FGuid InItemGuid)
{
	int32 ConsumableIndex = -1;
	FPrimaryAssetId ConsumableAssetId;
	InventoryComponent->GetAssetIdAndIndexByGuid(InItemGuid, ConsumableIndex, ConsumableAssetId);
	
	if (!ConsumableAssetId.IsValid()) return;
	
	TWeakObjectPtr<AMyCharacter> WeakThis = this;
	
	// Bind callback to OnLoadedDelegate, use Consumable when Consumable Asset loaded
	FOnAssetLoadedDelegate OnLoadedDelegate;
	OnLoadedDelegate.BindLambda([WeakThis, InItemGuid](UPrimaryDataAsset* LoadedAsset)
	{
		AMyCharacter* StrongThis = WeakThis.Get();
		if (!StrongThis) return;
		
		UConsumableDataAsset* ConsumableAsset = Cast<UConsumableDataAsset>(LoadedAsset);
		if (!ConsumableAsset || !StrongThis->AbilitySystemComponent) return;

		// Apply Effect
		const FGameplayEffectSpecHandle SpecHandle = StrongThis->AbilitySystemComponent->MakeOutgoingSpec(ConsumableAsset->Effect, 1, StrongThis->AbilitySystemComponent->MakeEffectContext());
		StrongThis->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
          
		StrongThis->InventoryComponent->AddItemByGuid(InItemGuid, -1);
	});

	// Get Asset
	InventoryComponent->GetPrimaryAssetAsync(ConsumableAssetId, OnLoadedDelegate);
}

void AMyCharacter::LoadFromSaveData()
{
	if (!HasAuthority()) return;
	
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetController());
	if (!GameInstance) return;
	
	const UMySaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UMySaveSubsystem>();
	if (!SaveSubsystem) return;
	
	if (!SaveSubsystem->bIsLoadingGame) return;
	if (!UGameplayStatics::DoesSaveGameExist(SaveSubsystem->CurrentSlotName, SaveSubsystem->CurrentSlotIndex)) return;
	
	UMySaveGame* LoadedSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSubsystem->CurrentSlotName, SaveSubsystem->CurrentSlotIndex));
	if (!LoadedSaveGame) return;
	
	// Load Attributes and Stats
	AMyPlayerState* MyPS = GetPlayerState<AMyPlayerState>();
	MyPS->SetLevel(LoadedSaveGame->PlayerLevel);
	MyPS->SetXP(LoadedSaveGame->XP);
	MyPS->AddToAttributePoints(LoadedSaveGame->AttributePoints);
			
	UMyAttributeSet* MyAS = Cast<UMyAttributeSet>(AttributeSet);
	check(MyAS)
	MyAS->SetStrength(LoadedSaveGame->Strength);
	MyAS->SetDexterity(LoadedSaveGame->Dexterity);
	MyAS->SetVitality(LoadedSaveGame->Vitality);
	
	// Empty Inventory
	InventoryComponent->RemoveAll();
	
	TArray<FPrimaryAssetId> AssetsToLoad;
    
	// Collect all Asset IDs that need to be loaded.
	for (const FItemData& It : LoadedSaveGame->SavedInventory)
	{
		if (It.AssetId.IsValid())
		{
			AssetsToLoad.Add(It.AssetId);
		}
	}

	if (AssetsToLoad.Num() == 0) return;
	
	TWeakObjectPtr<AMyCharacter> WeakThis = this;
	
	UAssetManager& AssetManager = UAssetManager::Get();
	// Only Asynchronous Loading one time to Avoid I/O performance jitter
	AssetManager.LoadPrimaryAssets(AssetsToLoad, TArray<FName>(), FStreamableDelegate::CreateLambda([WeakThis, SavedInventory = LoadedSaveGame->SavedInventory]()
	{
		AMyCharacter* StrongThis = WeakThis.Get();
		if (!StrongThis) return;
		
		for (const FItemData& It : SavedInventory)
		{
			StrongThis->InventoryComponent->AddItemByPrimaryAssetId(It.AssetId, It.Quantity);
                
			if (It.bIsEquipped)
			{
				// Equip Item
				if (auto Entries = StrongThis->InventoryComponent->GetAllEntries(); Entries.Num() > 0)
				{
					StrongThis->EquipWeapon(Entries.Last().ItemGuid);
				}
			}
		}
	}));
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCharacter::AddStartupItems()
{
	if (!HasAuthority()) return;
	
	// If is Load from Save Slot, no need to add startup items
	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetController()))
	{
		if (const UMySaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UMySaveSubsystem>())
		{
			if (SaveSubsystem->bIsLoadingGame) return; 
		}
	}
	
	check(InventoryComponent);
	for (const auto& ItemAssetId : StartupItemAssetIds)
	{
		InventoryComponent->AddItemByPrimaryAssetId(ItemAssetId, 1);
	}
}

void AMyCharacter::InitAbilityActorInfo()
{
	AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>();
	check(MyPlayerState);
	MyPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MyPlayerState, this);
	AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
	AttributeSet = MyPlayerState->GetAttributeSet();
	
	InitializeDefaultAttributes();
	AddCharacterAbilities();
	AddCharacterAbilitiesAndActivate();
	ApplyCharacterDefaultEffects();
	AddStartupItems();
	BindMoveSpeed();
	
	if (!IsLocallyControlled()) return;
	
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetController());
	if (!MyPlayerController) return;
	
	AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD());
	if (!MyHUD) return;
	
	MyHUD->InitCharacterStatusViewModel(MyPlayerController, MyPlayerState, AbilitySystemComponent, AttributeSet);
	MyHUD->InitInventoryViewModel(MyPlayerController, MyPlayerState, AbilitySystemComponent, AttributeSet, this);
	MyHUD->InitSaveGameViewModel(MyPlayerController, MyPlayerState, AbilitySystemComponent, AttributeSet);
	MyHUD->InitOverlayUI();
	MyHUD->InitMenuUI();
	MyHUD->InitSaveGameUI();
}
