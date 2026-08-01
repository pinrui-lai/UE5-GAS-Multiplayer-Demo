#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGame/VM_SaveGame.h"
#include "MySaveSubsystem.generated.h"

class UMySaveGame;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSaveDataChanged, const FString&, SlotName, int32, SlotIndex);

UCLASS()
class MYPROJECT_API UMySaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	FOnSaveDataChanged OnSaveDataChangedDelegate;
	
	void SaveGameSlot(const FString& SlotName, int32 SlotIndex, const FMySaveDataStruct& SaveData) const;
	static UMySaveGame* LoadGameSlot(const FString& SlotName, int32 SlotIndex);
	
	// Record which slot to load
	FString CurrentSlotName;
	int32 CurrentSlotIndex;
	bool bIsLoadingGame = false;
};
