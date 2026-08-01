#include "AbilitySystem/Abilities/MyParryBlockAbility.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/MyChargeComboAbility.h"
#include "AbilitySystem/Tasks/AbilityTask_StartWarp.h"
#include "Game/MyGameplayTags.h"

class UAbilityTask_PlayMontageAndWait;

void UMyParryBlockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// Reset bIsAlreadyReleased to be able to trigger Block Ability
	bIsAlreadyReleased = false;
	
	// Warp Task for motion warping while a short time after "ChargeMontage" started
	if (WarpTargetName.IsValid())
	{
		UAbilityTask_StartWarp* StartWarpTask = UAbilityTask_StartWarp::CreateUAbilityTask_AbilityTask_StartWarp(this, WarpTargetName);
		StartWarpTask->ReadyForActivation();
	}
	
	// Listen for CounterSucceedEvent only if CounterAbilityTag.IsValid()
	if (CounterAbilityTag.IsValid())
	{
		UAbilityTask_WaitGameplayEvent* WaitCounterSucceedEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, MyGameplayTags::Event_CounterSucceed);
		WaitCounterSucceedEventTask->EventReceived.AddDynamic(this, &UMyParryBlockAbility::OnCounterSucceedEventReceived);
		WaitCounterSucceedEventTask->ReadyForActivation();
	}
	
	// Listen for Ability Input Release
	if (UAbilityTask_WaitInputRelease* WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this))
	{
		WaitInputReleaseTask->OnRelease.AddDynamic(this, &UMyParryBlockAbility::OnInputReleased);
		WaitInputReleaseTask->ReadyForActivation();	
	}
	
	// Listen for Window.Combo tag from ANS
	UAbilityTask_WaitGameplayTagAdded* WaitWindowComboTagAddTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, MyGameplayTags::Window_Combo);
	WaitWindowComboTagAddTask->Added.AddDynamic(this, &UMyParryBlockAbility::OnWindowComboTagAdded);
	WaitWindowComboTagAddTask->ReadyForActivation();
	
	// Play AbilityMontage
	if (AbilityMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageAndWait"), AbilityMontage);
		PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UMyParryBlockAbility::OnMontageFinished);
		PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UMyParryBlockAbility::OnMontageFinished);
		PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UMyParryBlockAbility::OnMontageFinished);
		PlayMontageAndWaitTask->ReadyForActivation();	
	}
}

void UMyParryBlockAbility::OnCounterSucceedEventReceived(FGameplayEventData Payload)
{
	MyAbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(CounterAbilityTag));
}

void UMyParryBlockAbility::OnInputReleased(float TimeHeld)
{
	bIsAlreadyReleased = true;
}

void UMyParryBlockAbility::OnWindowComboTagAdded()
{
	if (!bIsAlreadyReleased)
	{
		MyAbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(BlockAbilityTag));	
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UMyParryBlockAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
