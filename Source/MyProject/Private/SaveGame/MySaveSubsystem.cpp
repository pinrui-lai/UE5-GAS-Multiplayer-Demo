#include "SaveGame/MySaveSubsystem.h"

#include "SaveGame/MySaveGame.h"
#include "Kismet/GameplayStatics.h"

void UMySaveSubsystem::SaveGameSlot(const FString& SlotName, const int32 SlotIndex, const FMySaveDataStruct& SaveData) const
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if (!SaveGameInstance) return;
	
	SaveGameInstance->SaveTime = SaveData.SaveTime;
		
	SaveGameInstance->PlayerLevel = SaveData.PlayerLevel;;
	SaveGameInstance->XP = SaveData.XP;
	SaveGameInstance->AttributePoints = SaveData.AttributePoints;

	SaveGameInstance->Strength = SaveData.Strength;
	SaveGameInstance->Dexterity = SaveData.Dexterity;
	SaveGameInstance->Vitality = SaveData.Vitality;
		
	SaveGameInstance->SavedInventory = SaveData.SavedInventory;
		
	SaveGameInstance->SavedLevelPath = SaveData.SavedLevelPath;

	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, SlotIndex))
	{
		OnSaveDataChangedDelegate.Broadcast(SlotName, SlotIndex);
	}
}

UMySaveGame* UMySaveSubsystem::LoadGameSlot(const FString& SlotName, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		return Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	}
	return nullptr;
}
