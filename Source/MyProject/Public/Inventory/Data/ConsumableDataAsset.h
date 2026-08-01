#pragma once

#include "CoreMinimal.h"
#include "Inventory/Data/ItemDataAsset.h"
#include "ConsumableDataAsset.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UConsumableDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly, Category = "UI")
	TSubclassOf<UGameplayEffect> Effect;
};
