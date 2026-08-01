#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "DamageNumberCue.generated.h"

/**
 * Displays damage numbers.
 */
UCLASS()
class MYPROJECT_API UDamageNumberCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
