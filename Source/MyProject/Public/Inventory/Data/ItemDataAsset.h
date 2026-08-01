#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Item,
	Consumable,
	Weapon,
};

UCLASS()
class MYPROJECT_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly, Category = "UI")
	FText ItemName;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly, Category = "UI")
	EItemType ItemType;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		// The first parameter here, "ItemDataAsset", must be the same as the Primary Asset Type in Project Settings
		// GetFName() will return "DA_Sword_01" for example
		return FPrimaryAssetId("ItemDataAsset", GetFName());
	}
};
