#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "HitNiagaraCue.generated.h"

/**
 * Displays the effect of hitting.
 */
UCLASS()
class MYPROJECT_API UHitNiagaraCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UHitNiagaraCue();
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
