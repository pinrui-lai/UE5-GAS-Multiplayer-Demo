#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TagToComboTable.generated.h"

class UComboDataAsset;

USTRUCT()
struct FTagToComboTableStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponTypeTag;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UComboDataAsset> ComboDataAsset;
};

/**
 * Use this table to get corresponding Combo Table when change weapon.
 */
UCLASS()
class MYPROJECT_API UTagToComboTable : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UComboDataAsset* GetComboTableByTag(FGameplayTag InWeaponTypeTag);
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FTagToComboTableStruct> TagToComboTables;
};
