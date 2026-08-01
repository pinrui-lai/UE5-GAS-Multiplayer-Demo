#include "SaveGame/VM_SaveGame.h"

#include "AbilitySystem/MyAttributeSet.h"
#include "SaveGame/MySaveGame.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryComponent.h"
#include "SaveGame/MySaveSubsystem.h"
#include "Inventory/MyFastArray.h"

void UVM_SaveGame::BindToDependencies()
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(PC.Get());
	if (!GameInstance) return;

	UMySaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UMySaveSubsystem>();
	if (!SaveSubsystem) return;
		
	SaveSubsystem->OnSaveDataChangedDelegate.AddDynamic(this, &UVM_SaveGame::OnSaveDataChangedCallback);
		
	// Broadcast for first time
	SaveSubsystem->OnSaveDataChangedDelegate.Broadcast(SlotName, SlotIndex);
}

void UVM_SaveGame::InitializeViewModel(const FString& InSlotName, const int32 InSlotIndex)
{
	SlotName = InSlotName;
	SlotIndex = InSlotIndex;
}

void UVM_SaveGame::OnSaveDataChangedCallback(const FString& InSlotName, const int32 InSlotIndex)
{
	if (InSlotName != SlotName || InSlotIndex != SlotIndex) return;
	
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex)) return;
	
	const UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	if (!LoadedGame) return;
	
	UE_MVVM_SET_PROPERTY_VALUE(SaveTime, LoadedGame->SaveTime);
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, LoadedGame->PlayerLevel);
	UE_MVVM_SET_PROPERTY_VALUE(AttributePoints, LoadedGame->AttributePoints);
	UE_MVVM_SET_PROPERTY_VALUE(Strength, LoadedGame->Strength);
	UE_MVVM_SET_PROPERTY_VALUE(Dexterity, LoadedGame->Dexterity);
	UE_MVVM_SET_PROPERTY_VALUE(Vitality, LoadedGame->Vitality);
}


void UVM_SaveGame::SaveGameSlot() const
{
	const UMyAttributeSet* MyAS = Cast<UMyAttributeSet>(AS);
	if (!MyAS) return;
	
	if (!PC.IsValid()) return;
	
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(PC->GetPawn());
	if (!PlayerInterface) return;
	
	UInventoryComponent* InventoryComponent = PC->GetPawn()->FindComponentByClass<UInventoryComponent>();
	if (!InventoryComponent) return;
	
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(PC.Get());
	if (!GameInstance) return;
	
	const UMySaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UMySaveSubsystem>();
	if (!SaveSubsystem) return;
	
	// Prepare data
	const FDateTime CurrentTime = FDateTime::Now();
	const FString TimeString = CurrentTime.ToString(TEXT("%Y/%m/%d - %H:%M:%S"));

	const int32 InPlayerLevel = PlayerInterface->GetPlayerLevel();
	const int32 InXP = PlayerInterface->GetXP();
	const int32 InAttributePoints = PlayerInterface->GetAttributePoints();

	const float InStrength = MyAS->GetStrength();
	const float InDexterity = MyAS->GetDexterity();
	const float InVitality = MyAS->GetVitality();
		
	const TArray<FInventoryEntry>& Entries = InventoryComponent->GetAllEntries();
	TArray<FItemData> InSavedInventory;
	for (int32 i = 0; i < Entries.Num(); i++)
	{
		InSavedInventory.Add(FItemData(Entries[i].ItemAssetId, Entries[i].Quantity, Entries[i].bIsEquipped));
	}
			
	FSoftObjectPath SavedLevelPath;
	if (UWorld* CurrentWorld = GetWorld())
	{
		SavedLevelPath = FSoftObjectPath(CurrentWorld);
	}
		
	// Save to slot
	SaveSubsystem->SaveGameSlot(SlotName, SlotIndex, FMySaveDataStruct(TimeString, InPlayerLevel, InXP, InAttributePoints, InStrength, InDexterity, InVitality, InSavedInventory, SavedLevelPath));
}

void UVM_SaveGame::LoadGameSlot() const
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(PC.Get());
	if (!GameInstance) return;
	
	UMySaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UMySaveSubsystem>();
	if (!SaveSubsystem) return;
	
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex)) return;
	
	const UMySaveGame* LoadedSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	SaveSubsystem->CurrentSlotName = SlotName;
	SaveSubsystem->CurrentSlotIndex = SlotIndex;
	SaveSubsystem->bIsLoadingGame = true;

	const TSoftObjectPtr<UWorld> LevelToOpen(LoadedSaveGame->SavedLevelPath);
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelToOpen);
}

void UVM_SaveGame::SetSaveTime(const FString& InSaveTime)
{
	UE_MVVM_SET_PROPERTY_VALUE(SaveTime, InSaveTime);
}

void UVM_SaveGame::SetPlayerLevel(const int32 InPlayerLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InPlayerLevel);
}

void UVM_SaveGame::SetAttributePoints(const int32 InAttributePoints)
{
	UE_MVVM_SET_PROPERTY_VALUE(AttributePoints, InAttributePoints);
}

void UVM_SaveGame::SetStrength(const float InStrength)
{
	UE_MVVM_SET_PROPERTY_VALUE(Strength, InStrength);
}

void UVM_SaveGame::SetDexterity(const float InDexterity)
{
	UE_MVVM_SET_PROPERTY_VALUE(Dexterity, InDexterity);
}

void UVM_SaveGame::SetVitality(const float InVitality)
{
	UE_MVVM_SET_PROPERTY_VALUE(Vitality, InVitality);
}
