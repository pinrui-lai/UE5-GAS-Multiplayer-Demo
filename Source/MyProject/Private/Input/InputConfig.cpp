#include "Input/InputConfig.h"

UInputAction* UInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag) const
{
	for (const FAbilityInputAction& AbilityInputAction : AbilityInputActions)
	{
		if (AbilityInputAction.InputAction && AbilityInputAction.InputTag == InputTag)
		{
			return AbilityInputAction.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	return nullptr;
}
