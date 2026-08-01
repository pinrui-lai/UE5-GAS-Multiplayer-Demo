#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "MyAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
