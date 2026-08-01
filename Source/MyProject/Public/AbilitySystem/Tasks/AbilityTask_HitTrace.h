#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_HitTrace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitTraceHitDelegate, const FGameplayAbilityTargetDataHandle&, HitResult);
/**
 * Do trace between "current(this frame) socket location" and "previous(last frame) socket location".
 * Only do trace at client side and sent result to server to reduce lag.
 */
UCLASS()
class MYPROJECT_API UAbilityTask_HitTrace : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	explicit UAbilityTask_HitTrace(const FObjectInitializer& Initializer);
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_HitTrace* CreateUHitTraceAbilityTask(UGameplayAbility* OwningAbility,
		USkeletalMeshComponent* WeaponMesh,
		TArray<FName> SocketNames,
		ECollisionChannel CollisionChannel,
		float SphereRadius = 0.f);
	
	UPROPERTY()
	FHitTraceHitDelegate OnTraceHit;
	
	virtual void TickTask(float DeltaTime) override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	virtual void Activate() override;
	
private:
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag) const;
	
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> WeaponMechComponent;
	
	TArray<FName> SocketNames;
	ECollisionChannel CollisionChannel;
	
	float SphereRadius;
	
	TArray<FVector> PreviousSocketLocations;
	bool bIsFirstFrame;
	
	UPROPERTY()
	TArray<TObjectPtr<AActor>> AlreadyHitActors;
};
