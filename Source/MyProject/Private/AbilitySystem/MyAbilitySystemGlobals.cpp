#include "AbilitySystem/MyAbilitySystemGlobals.h"

#include "MyAbilityTypes.h"

FGameplayEffectContext* UMyAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FMyGameplayEffectContext();
}
