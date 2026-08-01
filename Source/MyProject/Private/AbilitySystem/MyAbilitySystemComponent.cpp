#include "AbilitySystem/MyAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/MyCharacter.h"
#include "GameplayTagContainer.h"
#include "Game/MyGameplayTags.h"
#include "AbilitySystem/Data/ComboDataAsset.h"

void UMyAbilitySystemComponent::UpgradeAttribute(const FGameplayTag AttributeTag)
{
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetAvatarActor());
	check(PlayerInterface)
	
	if (PlayerInterface->GetAttributePoints() > 0)
	{
		ServerUpgradeAttribute(AttributeTag);
	}
}

void UMyAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	
	// Player has a passive ability that will make a GE to add attribute when receive event
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	
	// Reduce Attribute Points on server.
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetAvatarActor()))
	{
		PlayerInterface->AddToAttributePoints(-1);
	}
}

void UMyAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses)
{
	for (const auto& AbilityClass : AbilityClasses)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbility(AbilitySpec);
	}
}

void UMyAbilitySystemComponent::AddCharacterAbilitiesAndActivate(
	const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses)
{
	for (const auto& AbilityClass : AbilityClasses)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UMyAbilitySystemComponent::AbilityInputPressed(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	
	FGameplayAbilitySpec* AbilitySpecPtr = nullptr;
	if (InputTag == MyGameplayTags::Input_Run)
	{
		AbilitySpecPtr = FindAbilitySpecPtrByAssetTag(MyGameplayTags::Ability_Active_Common_Run);
	}
	else if (InputTag == MyGameplayTags::Input_Roll)
	{
		AbilitySpecPtr = FindAbilitySpecPtrByAssetTag(MyGameplayTags::Ability_Active_Common_Roll);
	}
	// Combo Ability Input
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, InputTag.GetTagName().ToString());
		if (HasAnyMatchingGameplayTags(FGameplayTagContainer(MyGameplayTags::Window_PreInput)))
		{
			PreInputTagBuffer = InputTag;
			return;
		}
		
		if (HasAnyMatchingGameplayTags(FGameplayTagContainer(MyGameplayTags::Window_Combo)))
		{
			if (!ComboDataAsset) return;
			AbilitySpecPtr= ProcessComboInput(InputTag);	
		}
	}
	
	
	
	if (!AbilitySpecPtr) return;
	
	if (!AbilitySpecPtr->IsActive())
	{
		if (TryActivateAbility(AbilitySpecPtr->Handle))
		{
			AbilitySpecInputPressed(*AbilitySpecPtr);
			
			// Get all currently executing instance
			TArray<UGameplayAbility*> Instances = AbilitySpecPtr->GetAbilityInstances();
			if (Instances.Num() <= 0) return;
	
			// Get the current prediction key for the latest executing instance.
			const FPredictionKey InstantiationPredictionKey = Instances.Last()->GetCurrentActivationInfo().GetActivationPredictionKey();
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpecPtr->Handle, InstantiationPredictionKey);
		}
	}
}

void UMyAbilitySystemComponent::AbilityInputHeld(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	
	// Allows players to hold the Run Key during an attack, and immediately Run after the attack ends.
	if (InputTag != MyGameplayTags::Input_Run) return;
	
	FGameplayAbilitySpec* AbilitySpecPtr = FindAbilitySpecPtrByAssetTag(MyGameplayTags::Ability_Active_Common_Run);
	if (!AbilitySpecPtr) return;
	
	if (!AbilitySpecPtr->IsActive())
	{
		if (TryActivateAbility(AbilitySpecPtr->Handle))
		{
			AbilitySpecInputPressed(*AbilitySpecPtr);
			
			// Get all currently executing instance
			TArray<UGameplayAbility*> Instances = AbilitySpecPtr->GetAbilityInstances();
			if (Instances.Num() <= 0) return;
	
			// Get the current prediction key for the latest executing instance.
			const FPredictionKey InstantiationPredictionKey = Instances.Last()->GetCurrentActivationInfo().GetActivationPredictionKey();
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpecPtr->Handle, InstantiationPredictionKey);
		}
	}
}

void UMyAbilitySystemComponent::AbilityInputReleased(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	
	FGameplayAbilitySpec* AbilitySpecPtr(nullptr);
	if (InputTag == MyGameplayTags::Input_Run)
	{
		AbilitySpecPtr = FindAbilitySpecPtrByAssetTag(MyGameplayTags::Ability_Active_Common_Run);
	}
	else if (InputTag == MyGameplayTags::Input_Roll)
	{
		AbilitySpecPtr = FindAbilitySpecPtrByAssetTag(MyGameplayTags::Ability_Active_Common_Roll);
	}
	else
	{
		if (!ComboDataAsset) return;
		
		// Get active ability and check if it's asset tag has corresponding input tag that equals parameter InputTag
		for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
		{
			if (!Spec.IsActive()) continue; 
			
			const FGameplayTagContainer& AbilityTagContainer = Spec.Ability->GetAssetTags();
			if (AbilityTagContainer.IsEmpty()) continue;
			
			const FGameplayTag AbilityAssetTag = AbilityTagContainer.First();
			
			if (ComboDataAsset->FindInputTagByAbilityTag(AbilityAssetTag) == InputTag)
			{
				AbilitySpecPtr = FindAbilitySpecPtrByAssetTag(AbilityAssetTag);
				break;
			}
		}
	}
	
	if (!AbilitySpecPtr) return;
	AbilitySpecInputReleased(*AbilitySpecPtr);
	
	if (!AbilitySpecPtr->IsActive()) return;
	
	TArray<UGameplayAbility*> Instances = AbilitySpecPtr->GetAbilityInstances();
	if (Instances.Num() <= 0) return;
	
	FPredictionKey InstantiationPredictionKey = Instances.Last()->GetCurrentActivationInfo().GetActivationPredictionKey();
	InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpecPtr->Handle, InstantiationPredictionKey);
}

FGameplayAbilitySpec*  UMyAbilitySystemComponent::FindAbilitySpecPtrByAssetTag(const FGameplayTag AssetTag)
{
	FScopedAbilityListLock ScopedAbilityListLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->GetAssetTags().HasTagExact(AssetTag))
		{
			FGameplayAbilitySpec* SpecPtr = &Spec;
			return SpecPtr;
		}
	}
	return nullptr;
}

FGameplayAbilitySpec* UMyAbilitySystemComponent::ProcessComboInput(const FGameplayTag InputTag)
{
	const FGameplayTag AbilityTagToActivate = ComboDataAsset->FindAbilityTag(InputTag, CurrentStateTag);
	return FindAbilitySpecPtrByAssetTag(AbilityTagToActivate);
}

void UMyAbilitySystemComponent::OnWindowPreInputTagChanged(FGameplayTag InputTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PreInputTagBuffer = FGameplayTag();
	}
}

void UMyAbilitySystemComponent::OnWindowComboTagChanged(FGameplayTag InputTag, int32 NewCount)
{
	if (NewCount > 0 && PreInputTagBuffer.IsValid())
	{
		const FGameplayTag InputTagToExecute = PreInputTagBuffer;
		PreInputTagBuffer = FGameplayTag();
		
		AbilityInputPressed(InputTagToExecute);
	}
}

void UMyAbilitySystemComponent::OnStateCombatTagChanged(FGameplayTag InputTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		AddLooseGameplayTag(MyGameplayTags::Window_Combo);
		// Window_Combo will be removed when Combo Ability Activated, and will be added by AnimNotifyState in Montage.
	}
}

void UMyAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterGameplayTagEvent(MyGameplayTags::Window_PreInput, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	RegisterGameplayTagEvent(MyGameplayTags::Window_PreInput, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UMyAbilitySystemComponent::OnWindowPreInputTagChanged);
	
	RegisterGameplayTagEvent(MyGameplayTags::Window_Combo, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	RegisterGameplayTagEvent(MyGameplayTags::Window_Combo, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UMyAbilitySystemComponent::OnWindowComboTagChanged);
	
	RegisterGameplayTagEvent(MyGameplayTags::State_Combat, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	RegisterGameplayTagEvent(MyGameplayTags::State_Combat, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UMyAbilitySystemComponent::OnStateCombatTagChanged);
	
	AddLooseGameplayTag(MyGameplayTags::Window_Combo);
}
