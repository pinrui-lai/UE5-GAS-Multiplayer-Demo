#pragma once

#include "CoreMinimal.h"
#include "MySaveGame.h"
#include "UI/VM_MyViewModelBase.h"
#include "VM_SaveGame.generated.h"

/**
 * To display `Load Screen UI in MainMenu` and `Save Screen UI in game`.
 */
USTRUCT()
struct FMySaveDataStruct
{
	GENERATED_BODY()
	
public:
	FMySaveDataStruct() : SaveTime(FString("NoData")), PlayerLevel(0), XP(0), AttributePoints(0), 
	Strength(0), Dexterity(0), Vitality(0), SavedInventory(TArray<FItemData>()){}
	
	FMySaveDataStruct(const FString& InSaveTime, const int32 InPlayerLevel, const int32 InXP,
	const int32 InAttributePoints, const float InStrength, const float InDexterity, const float InVitality,
	const TArray<FItemData>& InSavedInventory, const FSoftObjectPath& SavedLevelPath) : 
	SaveTime(InSaveTime), PlayerLevel(InPlayerLevel), XP(InXP), AttributePoints(InAttributePoints), 
	Strength(InStrength), Dexterity(InDexterity), Vitality(InVitality), SavedInventory(InSavedInventory), SavedLevelPath(SavedLevelPath)
	{}
	
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

UCLASS()
class MYPROJECT_API UVM_SaveGame : public UVM_MyViewModelBase
{
	GENERATED_BODY()
	
public:
	void BindToDependencies();
	
	// Let the view model know which save slot it is associated with.
	void InitializeViewModel(const FString& InSlotName, int32 InSlotIndex);
	
	UFUNCTION()
	void OnSaveDataChangedCallback(const FString& InSlotName, int32 InSlotIndex);
	
	// Prepare data then call subsystem's SaveGameSlot
	UFUNCTION(BlueprintCallable)
	void SaveGameSlot() const;
	
	// Get Save Slot associated with self, then open saved level
	UFUNCTION(BlueprintCallable)
	void LoadGameSlot() const;
	
	void SetSaveTime(const FString& InSaveTime);
	void SetPlayerLevel(int32 InPlayerLevel);
	void SetAttributePoints(int32 InAttributePoints);
	void SetStrength(float InStrength);
	void SetDexterity(float InDexterity);
	void SetVitality(float InVitality);
	
	FString GetSaveTime() const { return SaveTime; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
	int32 GetAttributePoints() const { return AttributePoints; }
	float GetStrength() const { return Strength; }
	float GetDexterity() const { return Dexterity; }
	float GetVitality() const { return Vitality; }
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	FString SaveTime;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	int32 PlayerLevel;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	int32 AttributePoints;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Strength;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Dexterity;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Vitality;

	FString SlotName;
	int32 SlotIndex;
};
