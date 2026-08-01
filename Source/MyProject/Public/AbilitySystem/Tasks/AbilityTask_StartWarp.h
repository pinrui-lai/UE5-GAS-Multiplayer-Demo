#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_StartWarp.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FGetInputRotationDelegate, const FGameplayAbilityTargetDataHandle& LocationTargetDataHandle);
/**
 * Get Last Movement Input Vector, calculate Warp Target's location then update Warp Target.
 */
UCLASS()
class MYPROJECT_API UAbilityTask_StartWarp : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	explicit UAbilityTask_StartWarp(const FObjectInitializer& Initializer);
	
	UFUNCTION(Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_StartWarp* CreateUAbilityTask_AbilityTask_StartWarp(UGameplayAbility* OwningAbility, FName WarpTargetName, float WarpDistance = 150.f);
	
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
	FGetInputRotationDelegate OnGetLastMovementInputTransform;

protected:
	virtual void Activate() override;
	
private:
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle, FGameplayTag ActivationTag) const;
	
	FName WarpTargetName;
	float WarpDistance;
};
