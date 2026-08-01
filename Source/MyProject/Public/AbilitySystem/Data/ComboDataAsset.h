#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ComboDataAsset.generated.h"

USTRUCT()
struct FComboInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CurrentStateTag;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag NextAbilityTag;
};

/**
 * Table for searching next ability in combo.
 */
UCLASS()
class MYPROJECT_API UComboDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FGameplayTag FindAbilityTag(const FGameplayTag InputTag, const FGameplayTag CurrentStateTag);
	FGameplayTag FindInputTagByAbilityTag(const FGameplayTag InAbilityTag);
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FComboInfo> ComboInfo;
};
