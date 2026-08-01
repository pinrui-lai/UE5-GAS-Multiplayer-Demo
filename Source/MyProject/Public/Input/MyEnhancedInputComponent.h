#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputConfig.h"
#include "MyEnhancedInputComponent.generated.h"

/**
 * Use BindAbilityActions() to bind all InputActions in InputConfig to PressedFunc, ReleasedFunc and HeldFunc, and pass it's corresponding Input Tags as parameter.
 */
UCLASS()
class MYPROJECT_API UMyEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	
	// Bind all InputActions in InputConfig to PressedFunc, ReleasedFunc and HeldFunc
	void BindAbilityActions(UInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UMyEnhancedInputComponent::BindAbilityActions(UInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const FAbilityInputAction& AbilityInputAction : InputConfig->AbilityInputActions)
	{
		if (AbilityInputAction.InputAction && AbilityInputAction.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(AbilityInputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, AbilityInputAction.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(AbilityInputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, AbilityInputAction.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(AbilityInputAction.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, AbilityInputAction.InputTag);
			}
			
		}
	}
}
