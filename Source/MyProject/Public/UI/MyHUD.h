#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UVM_SaveGame;
class UDamageNumberComponent;
class AMyCharacter;
class UVM_Inventory;
class UAttributeSet;
class UAbilitySystemComponent;
class UVM_CharacterStatus;
class UMyWidgetBase;
class UVM_Overlay;
/**
 * Manage View and View Model in game.
 */
UCLASS()
class MYPROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	void InitCharacterStatusViewModel(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void InitInventoryViewModel(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, const AMyCharacter* TargetCharacter);
	void InitSaveGameViewModel(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	void InitOverlayUI();
	void InitMenuUI();
	void InitSaveGameUI();
	
	bool OpenUI(UMyWidgetBase* UIWidget) const;
	bool OpenMenu() const;
	bool OpenSaveGameUI() const;
	
	bool CloseUI(UMyWidgetBase* UIWidget) const;
	bool CloseMenu() const;
	bool CloseSaveGameUI() const;
	
	void ToggleUI(UMyWidgetBase* UIWidget) const;
	void ToggleMenu() const;
	void ToggleSaveGameUI() const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyWidgetBase> OverlayWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyWidgetBase> MenuWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMyWidgetBase> SaveGameWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> DamageNumberWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVM_CharacterStatus> CharacterStatusViewModelClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVM_Inventory> InventoryViewModelClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVM_SaveGame> SaveGameViewModelClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageNumberComponent> DamageNumberComponentClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMyWidgetBase> OverlayWidget;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMyWidgetBase> MenuWidget;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMyWidgetBase> SaveGameWidget;

private:
	UPROPERTY()
	TObjectPtr<UVM_CharacterStatus> CharacterStatusViewModel;
	
	UPROPERTY()
	TObjectPtr<UVM_Inventory> InventoryViewModel;
	
	UPROPERTY()
	TObjectPtr<UVM_SaveGame> SaveGameViewModel1;
	
	UPROPERTY()
	TObjectPtr<UVM_SaveGame> SaveGameViewModel2;
	
	UPROPERTY()
	TObjectPtr<UVM_SaveGame> SaveGameViewModel3;
	
	UPROPERTY()
	TArray<UVM_SaveGame*> SaveGameViewModels;
	
};
