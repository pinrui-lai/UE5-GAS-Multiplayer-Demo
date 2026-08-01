#pragma once

#include "CoreMinimal.h"
#include "MyAbility.h"
#include "Abilities/GameplayAbility.h"
#include "MyChargeComboAbility.generated.h"

class UAbilityTask_HitTrace;
class UMyAbilitySystemComponent;
class UAbilityTask_WaitInputRelease;
/**
 * UMyChargeComboAbility
 * * A comprehensive Gameplay Ability designed for handling "Combo Executions" and "Charged Attacks".
 * It integrates multi-stage charging mechanisms, Motion Warping, AnimNotifyState-driven weapon collision tracing, and dynamic damage scaling.
 * * Core Features & Flow:
 * 1. [Charge Stage Management] (Skip this step If `bIsChargeAttack` is false):
 * - If `bIsChargeAttack` is true, plays `ChargeMontage` and sets up timers to transition through Stage 1 to 4.
 * - Each stage advancement triggers a visual/audio GameplayCue. Releasing the input or reaching max stage automatically executes the attack.
 * * 2. [Attack Execution & Warping]:
 * - Executes the main `AbilityMontage` and utilizes `UAbilityTask_StartWarp` at key moments to realign the character toward the target.
 * * 3. [Dynamic Hit Tracing]:
 * - Listens for `Window_IsHitting` Gameplay Tag added via AnimNotifyState (ANS).
 * - Triggers weapon socket-based line/sphere tracing (`UAbilityTask_HitTrace`) while the tag is present, and terminates the task when removed.
 * * 4. [Hit Resolution & Scaled Damage]:
 * - Executes Hit-Stop GameplayCues on successfully striking a target.
 * - Evaluates the final `CurrentStage` to apply the corresponding `MotionValueMultiplier`.
 * - SetByCaller to inject the calculated MotionValue into GameplayEffect and apply it to the target.
 * * 5. [Event-Driven Extensions]:
 * - Listens for CounterSucceedEvent to trigger follow-up counter abilities (CounterSucceedEvent will occur when target has Window.Counter tag).
 * - Listens for WeaponAbilityEvent (Weapon Ability, e.g., spawning projectiles midway through a swing) to execute derived skills.
 */
UCLASS()
class MYPROJECT_API UMyChargeComboAbility : public UMyAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Motnage")
	TObjectPtr<UAnimMontage> AbilityMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Effect")
	TSubclassOf<UGameplayEffect> AbilityEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Motion Value")
	float MotionValue;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Trace")
	float SphereRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|MotionWarp")
	FName WarpTargetName = "WarpTarget";
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Counter")
	FGameplayTag CounterAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Charge")
	bool bIsChargeAttack;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Charge")
	TObjectPtr<UAnimMontage> ChargeMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Charge")
	float Stage2Time = 1.0f; 
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Charge")
	float Stage3Time = 2.0f; 
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Charge")
	float Stage4Time = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Charge")
	float Stage1MotionValueMultiplier = 1.0f; 
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Charge")
	float Stage2MotionValueMultiplier = 1.5f; 
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Charge")
	float Stage3MotionValueMultiplier = 2.0f; 
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Charge")
	float Stage4MotionValueMultiplier = 3.0f;

private:
	UFUNCTION()
	void OnInputReleased(float TimeHeld);
    
	// Callback for entering each stage
	void EnterStage2();
	void EnterStage3();
	void EnterStage4();
	
	// Play AttackMontage and attack
	UFUNCTION()
	void StartAttackExecution();
	
	void ClearAllTimers();
	
	UFUNCTION()
	void OnMontageFinished();
	
	UFUNCTION()
	void OnIsHittingTagAdded();
	
	UFUNCTION()
	void OnIsHittingTagRemoved();
	
	UFUNCTION()
	void OnWeaponHitTarget(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	
	UFUNCTION()
	void OnWeaponAbilityEventReceived(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnCounterSucceedEventReceived(FGameplayEventData Payload);
	
	int32 CurrentChargeStage = 1;

	// Timer for charge time of each stage
	FTimerHandle Stage2TimerHandle;
	FTimerHandle Stage3TimerHandle;
	FTimerHandle Stage4TimerHandle;
	
	// To EndTask manually
	UPROPERTY()
	TObjectPtr<UAbilityTask_HitTrace> HitTraceTask;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitInputRelease> WaitInputReleaseTask;
};
