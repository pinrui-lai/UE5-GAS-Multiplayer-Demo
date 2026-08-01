#pragma once

#include "CoreMinimal.h"
#include "MyAbility.h"
#include "MyParryBlockAbility.generated.h"

/**
 * UMyParryBlockAbility
 * * A specialized Gameplay Ability managing the Ability Flow between "Parry", "Block", and "Counter Attack".
 * It dynamically determines whether to activate Block Ability seamlessly based on player input duration and Window.Combo(from ANS).
 * * Core Features & Flow:
 * 1. [Initialization & Initialization]:
 * - Resets `bIsAlreadyReleased` to false upon activation, indicating the Ability Input is held.
 * - `UAbilityTask_StartWarp` to warp rotation during the parry startup.
 * * 2. [Task Listening & Event Handling]:
 * - [Counter Event]: Listens for `Event_CounterSucceed`. If a parry succeed, triggers the follow-up counter ability (`CounterAbilityTag`).
 * - [Input Release]: `UAbilityTask_WaitInputRelease` to detect when the player release Ability Input, setting `bIsAlreadyReleased` to true.
 * - [Combo Window]: Listens for the `Window_Combo` Gameplay Tag (from AnimNotifyState in the montage).
 * * 3. [Dynamic Parry-to-Block Transition]:
 * - Plays the initial parry `AbilityMontage`.
 * - When the animation reaches the Combo Window:
 *   - If the player is STILL holding the Ability Input (`bIsAlreadyReleased == false`), the ability automatically activates Block Ability (`BlockAbilityTag`) which will end this ability by `Cancel Ability with Tag`.
 *   - If the player has ALREADY released Ability Input, play the remaining Parry Ability Animation.
 * * 4. [Ability End]:
 * - Cleans up and ends the ability.
 */
UCLASS()
class MYPROJECT_API UMyParryBlockAbility : public UMyAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Motnage")
	TObjectPtr<UAnimMontage> AbilityMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|MotionWarp")
	FName WarpTargetName = "WarpTarget";
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Counter")
	FGameplayTag CounterAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Block")
	FGameplayTag BlockAbilityTag;
	
private:
	UFUNCTION()
	void OnCounterSucceedEventReceived(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnInputReleased(float TimeHeld);
	
	UFUNCTION()
	void OnWindowComboTagAdded();
	
	UFUNCTION()
	void OnMontageFinished();
	
	bool bIsAlreadyReleased;
};
