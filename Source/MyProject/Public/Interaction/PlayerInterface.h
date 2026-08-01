#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

class UWeaponDataAsset;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IPlayerInterface
{
	GENERATED_BODY()

public:
	virtual void AddToLevel(int32 InLevel) = 0;
	virtual void AddToXP(int32 InXP) = 0;
	virtual void AddToAttributePoints(int32 InAttributePoints) = 0;
	
	virtual int32 GetPlayerLevel() = 0;
	virtual int32 GetXP() = 0;
	virtual int32 GetAttributePoints() = 0;
	
	virtual int32 FindLevelByXP(int32 XP) = 0;
	virtual int32 GetAttributePointsRewardFromInfo(int32 Level) = 0;
	
	virtual void EquipWeapon(FGuid InItemGuid) = 0;
	virtual void UnequipWeapon() = 0;
	virtual void UseConsumable(FGuid InItemGuid) = 0;
	virtual void AddItemToInventory(FPrimaryAssetId AssetId, int32 Amount) = 0;
};
