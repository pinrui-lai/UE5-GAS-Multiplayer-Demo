#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyWidgetBase.generated.h"

class UVM_SaveGame;
class UVM_Enemy;
class UVM_Inventory;
class UVM_CharacterStatus;
/**
 * Passively receive View Model (HUD set View Models in this project).
 */
UCLASS()
class MYPROJECT_API UMyWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCharacterStatusViewModel(UVM_CharacterStatus* CharacterStatusViewModel);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInventoryViewModel(UVM_Inventory* InventoryViewModel);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetEnemyViewModel(UVM_Enemy* EnemyViewModel);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetSaveGameViewModel(const TArray<UVM_SaveGame*>& SaveGameViewModels);
};
