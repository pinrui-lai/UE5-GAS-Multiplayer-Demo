#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "HitStopCue.generated.h"

/**
 * Hit Stop when the attack hits.
 */
UCLASS()
class MYPROJECT_API UHitStopCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	UHitStopCue();
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "HitStop")
	float DefaultDuration = 0.1f;
	
	UPROPERTY(EditDefaultsOnly, Category = "HitStop")
	float MotionValueCoefficient = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "HitStop")
	float MinHitStopDuration = 0.02f;
	
	UPROPERTY(EditDefaultsOnly, Category = "HitStop")
	float MaxHitStopDuration = 0.2f;
	
	UPROPERTY(EditDefaultsOnly, Category = "HitStop")
	float HitStopTimeDilation = 0.01f;
	
	UPROPERTY(EditDefaultsOnly, Category = "HitStop")
	bool bDoesTargetStop = false;
};
