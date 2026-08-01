#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "HitSoundCue.generated.h"

/**
 * Play sound effect when attack hits. 
 */
UCLASS()
class MYPROJECT_API UHitSoundCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> SFXAsset;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> PerfectParriedSFXAsset;
};
