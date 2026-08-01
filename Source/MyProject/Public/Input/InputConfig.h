#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputConfig.generated.h"

struct FGameplayTag;
class UInputAction;

USTRUCT(BlueprintType)
struct FAbilityInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};


/**
 * Input Action and its corresponding Input Tag.
 */
UCLASS()
class MYPROJECT_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InputTag) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAbilityInputAction> AbilityInputActions;
};
