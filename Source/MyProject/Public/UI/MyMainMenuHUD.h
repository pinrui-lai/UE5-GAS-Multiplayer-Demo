#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyMainMenuHUD.generated.h"

class UVM_SaveGame;
class UMyWidgetBase;
/**
 * Manage View and View Model in Main Menu.
 */
UCLASS()
class MYPROJECT_API AMyMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void InitSaveGameViewModel();
	void InitMainMenuUI();
	void InitLoadScreenUI();
	
	UFUNCTION(BlueprintCallable)
	void DisplayMainMenuUI() const;
	
	UFUNCTION(BlueprintCallable)
	void DisplayLoadScreenUI() const;
	
	UFUNCTION(BlueprintCallable)
	void HideMainMenuUI() const;
	
	UFUNCTION(BlueprintCallable)
	void HideLoadScreenUI() const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyWidgetBase> MainMenuWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyWidgetBase> LoadScreenWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVM_SaveGame> SaveGameViewModelClass;
	
private:
	UPROPERTY()
	TObjectPtr<UMyWidgetBase> MainMenuWidget;
	
	UPROPERTY()
	TObjectPtr<UMyWidgetBase> LoadScreenWidget;
	
	UPROPERTY()
	TObjectPtr<UVM_SaveGame> SaveGameViewModel1;
	
	UPROPERTY()
	TObjectPtr<UVM_SaveGame> SaveGameViewModel2;
	
	UPROPERTY()
	TObjectPtr<UVM_SaveGame> SaveGameViewModel3;
	
	UPROPERTY()
	TArray<UVM_SaveGame*> SaveGameViewModels;
	
};
