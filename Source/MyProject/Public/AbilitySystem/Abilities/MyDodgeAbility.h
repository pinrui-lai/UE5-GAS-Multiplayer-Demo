#pragma once

#include "CoreMinimal.h"
#include "MyChargeComboAbility.h"
#include "Abilities/GameplayAbility.h"
#include "MyDodgeAbility.generated.h"

/**
 * UMyDodgeAbility
 * [Override CanActivateAbility] : Dodge can be activated in Combo Window or in idle state (no state tag).
 * [Four Directions Montages] : Which Montage to play determined by the angle between `Last Movement Input Vector` and the `character's forward vector`.
 */
UCLASS()
class MYPROJECT_API UMyDodgeAbility : public UMyAbility
{
	GENERATED_BODY()
	
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Montage")
	TObjectPtr<UAnimMontage> ForwardMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Montage")
	TObjectPtr<UAnimMontage> BackwardMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Montage")
	TObjectPtr<UAnimMontage> LeftMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Montage")
	TObjectPtr<UAnimMontage> RightMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|MotionWarping")
	FName WarpTargetName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|MotionWarping")
	float WarpDistance = 150.0f;
	
private:
	void OnGetLastMovementInputTransform(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	
	UFUNCTION()
	void OnMontageFinished();
};
