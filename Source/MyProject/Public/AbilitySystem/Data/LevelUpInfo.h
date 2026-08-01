#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * Level up information, contains "XP requirement to next level" and "Attribute Point reward when reach the level" of each level
 */
USTRUCT(BlueprintType)
struct FMyLevelUpInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement;
	
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointReward;
};

UCLASS()
class MYPROJECT_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FMyLevelUpInfo> LevelUpInformation;
	
	int32 FindLevelByXP(int32 XP) const;
};
