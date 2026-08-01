#include "AbilitySystem/Abilities/MyAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "Game/MyGameplayTags.h"

UMyAbility::UMyAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMyAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
	// Cache MyASC to set Current State Tag
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		MyAbilitySystemComponent = Cast<UMyAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());
	}
}

void UMyAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	
	// If is Combat Ability first remove Window_Combo to limit Combo Window.
	// Combo Window in Combat Ability will be managed by AnimNotifyState from montage.
	if (bIsCombatAbility)
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(MyGameplayTags::Window_Combo);	
	}
	
	// Set MyASC's CurrentStateTag fot searching next ability.
	const FGameplayTagContainer StateTags = ActivationOwnedTags.Filter(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State"))));
	check(MyAbilitySystemComponent)
	if (!StateTags.IsEmpty())
	{
		MyAbilitySystemComponent->SetCurrentStateTag(StateTags.First());
	}
}

void UMyAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Empty MyASC's CurrentStateTag.
	MyAbilitySystemComponent->SetCurrentStateTag(FGameplayTag());
	
	// AI Enemy will receive this Event and covert it to a State Tree Event, then send to itself for State Transition.
	FGameplayEventData Payload;
	Payload.EventTag = MyGameplayTags::Event_Ability_Lifecycle_End;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), MyGameplayTags::Event_Ability_Lifecycle_End, Payload);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
