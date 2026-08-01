#include "AbilitySystem/Abilities/MyChargeComboAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"
#include "AbilitySystem/Tasks/AbilityTask_StartWarp.h"
#include "Game/MyGameplayTags.h"
#include "Interaction/CombatInterface.h"

class ICombatInterface;

void UMyChargeComboAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
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
		WaitCounterSucceedEventTask->EventReceived.AddDynamic(this, &UMyChargeComboAbility::OnCounterSucceedEventReceived);
		WaitCounterSucceedEventTask->ReadyForActivation();
	}
	
	// Listen for IsHitting window tag from ANS, Only when Motion Value > 0 (has damage) need to do Trace
	if (MotionValue > 0)
	{
		UAbilityTask_WaitGameplayTagAdded* WaitIsHittingTagAddTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, MyGameplayTags::Window_IsHitting);
		WaitIsHittingTagAddTask->Added.AddDynamic(this, &UMyChargeComboAbility::OnIsHittingTagAdded);
		WaitIsHittingTagAddTask->ReadyForActivation();
	
		UAbilityTask_WaitGameplayTagRemoved* WaitIsHittingTagRemoveTask = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, MyGameplayTags::Window_IsHitting);
		WaitIsHittingTagRemoveTask->Removed.AddDynamic(this, &UMyChargeComboAbility::OnIsHittingTagRemoved);
		WaitIsHittingTagRemoveTask->ReadyForActivation();
	}
	
	// Listen for WeaponAbility event from AN (WeaponAbility is something like spawning fireball from sword swinging)
	UAbilityTask_WaitGameplayEvent* WaitWeaponAbilityEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, MyGameplayTags::Event_WeaponAbility);
	WaitWeaponAbilityEventTask->EventReceived.AddDynamic(this, &UMyChargeComboAbility::OnWeaponAbilityEventReceived);
	WaitWeaponAbilityEventTask->ReadyForActivation();
	
	// If is Charge Attack, Play ChargeMontage and use Timer to determine Charge Stage
	if (bIsChargeAttack)
	{
		CurrentChargeStage = 1;
		
		if (ChargeMontage)
		{
			UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageAndWait"), ChargeMontage);
			PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UMyChargeComboAbility::StartAttackExecution);
			PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UMyChargeComboAbility::StartAttackExecution);
			PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UMyChargeComboAbility::StartAttackExecution);
			PlayMontageAndWaitTask->ReadyForActivation();	
		}
		
		// Listen for Ability Input Release
		WaitInputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
		if (WaitInputReleaseTask)
		{
			WaitInputReleaseTask->OnRelease.AddDynamic(this, &UMyChargeComboAbility::OnInputReleased); // Execute Attack
			WaitInputReleaseTask->ReadyForActivation();	
		}
		
		// Set a Timer to time the charge time to determine Charge Stage
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.SetTimer(Stage2TimerHandle, this, &UMyChargeComboAbility::EnterStage2, Stage2Time, false);
		TimerManager.SetTimer(Stage3TimerHandle, this, &UMyChargeComboAbility::EnterStage3, Stage3Time, false);
		TimerManager.SetTimer(Stage4TimerHandle, this, &UMyChargeComboAbility::EnterStage4, Stage4Time, false);
	}
	else
	{
		// Not a Charge Attack, attack directly
		StartAttackExecution();
	}
}

void UMyChargeComboAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ClearAllTimers(); 
	
	if (WaitInputReleaseTask)
	{
		WaitInputReleaseTask->EndTask();
	}
	
	if (HitTraceTask)
	{
		HitTraceTask->EndTask();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMyChargeComboAbility::OnInputReleased(float TimeHeld)
{
	StartAttackExecution();
}

void UMyChargeComboAbility::EnterStage2()
{
	CurrentChargeStage = 2;
	FGameplayCueParameters CueParams;
	CueParams.RawMagnitude = 2;
	CueParams.Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(MyGameplayTags::GameplayCue_Combat_ChargeNiagara, CueParams);
}

void UMyChargeComboAbility::EnterStage3()
{
	CurrentChargeStage = 3;
	FGameplayCueParameters CueParams;
	CueParams.RawMagnitude = 3;
	CueParams.Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(MyGameplayTags::GameplayCue_Combat_ChargeNiagara, CueParams);
}

void UMyChargeComboAbility::EnterStage4()
{
	CurrentChargeStage = 4;
	FGameplayCueParameters CueParams;
	CueParams.RawMagnitude = 4;
	CueParams.Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(MyGameplayTags::GameplayCue_Combat_ChargeNiagara, CueParams);
	StartAttackExecution();
}

void UMyChargeComboAbility::StartAttackExecution()
{
	// Prevent receiving OnRelease again after "Auto Release (Attack) in EnterStage4".
	if (WaitInputReleaseTask)
	{
		WaitInputReleaseTask->EndTask();	
	}
	
	ClearAllTimers();
	
	// Warp Task for motion warping while a short time after "AttackMontage" started
	if (WarpTargetName.IsValid())
	{
		UAbilityTask_StartWarp* StartWarpTask = UAbilityTask_StartWarp::CreateUAbilityTask_AbilityTask_StartWarp(this, WarpTargetName);
		StartWarpTask->ReadyForActivation();
	}
	
	// Play AbilityMontage
	if (AbilityMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageAndWait"), AbilityMontage);
		PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UMyChargeComboAbility::OnMontageFinished);
		PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UMyChargeComboAbility::OnMontageFinished);
		PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UMyChargeComboAbility::OnMontageFinished);
		PlayMontageAndWaitTask->ReadyForActivation();	
	}
}

void UMyChargeComboAbility::ClearAllTimers()
{
	if (GetWorld())
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(Stage2TimerHandle);
		TimerManager.ClearTimer(Stage3TimerHandle);
		TimerManager.ClearTimer(Stage4TimerHandle);
	}
}

void UMyChargeComboAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMyChargeComboAbility::OnIsHittingTagAdded()
{
	const ICombatInterface* CombatInterface =  Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface) return;
	
	USkeletalMeshComponent* WeaponMeshComponent = CombatInterface->GetWeaponMeshComponent();
	const TArray<FName> WeaponSockets = CombatInterface->GetWeaponSocketNames();

	// Start Trace. If SphereRadius > 0 will do Sphere Trace, otherwise do Line Trace 
	HitTraceTask = UAbilityTask_HitTrace::CreateUHitTraceAbilityTask(this, WeaponMeshComponent, WeaponSockets, ECC_Pawn, SphereRadius);
	HitTraceTask->OnTraceHit.AddDynamic(this, &UMyChargeComboAbility::OnWeaponHitTarget);
	HitTraceTask->ReadyForActivation();
}

void UMyChargeComboAbility::OnIsHittingTagRemoved()
{
	if (HitTraceTask)
	{
		HitTraceTask->EndTask();
	}
}

void UMyChargeComboAbility::OnWeaponHitTarget(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	const FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor()))
	{
		// Multiply Motion Value according Charge Stage
		float MotionValueMultiplier(Stage1MotionValueMultiplier);
		if (CurrentChargeStage == 2) MotionValueMultiplier = Stage2MotionValueMultiplier;
		else if (CurrentChargeStage == 3)	MotionValueMultiplier = Stage3MotionValueMultiplier;
		else if (CurrentChargeStage == 4) MotionValueMultiplier = Stage4MotionValueMultiplier;
		
		// Hit Stop Cue.
		FGameplayCueParameters HitStopCueParams;
		HitStopCueParams.Instigator = GetAvatarActorFromActorInfo();
		HitStopCueParams.RawMagnitude = MotionValue * MotionValueMultiplier;
		TargetASC->ExecuteGameplayCue(MyGameplayTags::GameplayCue_Combat_HitStop, HitStopCueParams);
		
		// Prepare GE, HitNiagaraCue and DamageNumberCue are in GE's Gameplay Cues array.
		FGameplayEffectContextHandle ContextHandle = MyAbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddHitResult(HitResult);
		const FGameplayEffectSpecHandle Spec = MyAbilitySystemComponent->MakeOutgoingSpec(AbilityEffectClass, 1, ContextHandle);
		
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Spec, MyGameplayTags::SetByCaller_Ability_MotionValue.GetTag(), MotionValue * MotionValueMultiplier);
		MyAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
	}
}

void UMyChargeComboAbility::OnWeaponAbilityEventReceived(FGameplayEventData Payload)
{
	if (const ICombatInterface* CombatInterface =  Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		if (const FGameplayTag WeaponAbilityTag = CombatInterface->GetWeaponAbilityTag(); WeaponAbilityTag.IsValid())
		{
			MyAbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(WeaponAbilityTag));	
		}
	}
}

void UMyChargeComboAbility::OnCounterSucceedEventReceived(FGameplayEventData Payload)
{
	MyAbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(CounterAbilityTag));
}
