#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT()
struct FItemData
{
	GENERATED_BODY()
	
public:
	FItemData() : AssetId(FPrimaryAssetId()), Quantity(0), bIsEquipped(false){}
	FItemData(const FPrimaryAssetId& InAssetId, const int32 InQuantity, const bool InbIsEquipped) : AssetId(InAssetId), Quantity(InQuantity), bIsEquipped(InbIsEquipped) {}
	
	UPROPERTY()
	FPrimaryAssetId AssetId;
	
	UPROPERTY()
	int32 Quantity;
	
	UPROPERTY()
	bool bIsEquipped;
};

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FString SaveTime;
	
	UPROPERTY()
	int32 PlayerLevel;
	
	UPROPERTY()
	int32 XP;
	
	UPROPERTY()
	int32 AttributePoints;
	
	UPROPERTY()
	float Strength;
	
	UPROPERTY()
	float Dexterity;
	
	UPROPERTY()
	float Vitality;
	
	UPROPERTY()
	TArray<FItemData> SavedInventory;
	
	UPROPERTY()
	FSoftObjectPath SavedLevelPath;
};
