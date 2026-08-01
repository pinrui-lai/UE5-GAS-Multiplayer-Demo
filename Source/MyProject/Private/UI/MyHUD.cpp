#include "UI/MyHUD.h"

#include "Blueprint/UserWidget.h"
#include "DamageNumber/DamageNumberPoolSubsystem.h"
#include "UI/MyWidgetBase.h"
#include "UI/VM_CharacterStatus.h"
#include "SaveGame/VM_SaveGame.h"
#include "Inventory/VM_Inventory.h"

void AMyHUD::InitCharacterStatusViewModel(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
                                          UAttributeSet* AS)
{
	check(CharacterStatusViewModelClass)

	const FViewModelParams Params(PC, PS, ASC, AS);
	CharacterStatusViewModel = NewObject<UVM_CharacterStatus>(this, CharacterStatusViewModelClass);
	CharacterStatusViewModel->SetViewModelParams(Params);
	CharacterStatusViewModel->BindToDependencies();
}

void AMyHUD::InitInventoryViewModel(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS, const AMyCharacter* TargetCharacter)
{
	check(InventoryViewModelClass)

	const FViewModelParams Params(PC, PS, ASC, AS);
	InventoryViewModel = NewObject<UVM_Inventory>(this, InventoryViewModelClass);
	InventoryViewModel->SetViewModelParams(Params);
	InventoryViewModel->BindToDependencies(TargetCharacter);
}

void AMyHUD::InitSaveGameViewModel(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	check(SaveGameViewModelClass)

	const FViewModelParams Params(PC, PS, ASC, AS);
	SaveGameViewModel1 = NewObject<UVM_SaveGame>(this, SaveGameViewModelClass);
	SaveGameViewModel1->SetViewModelParams(Params);
	SaveGameViewModel1->InitializeViewModel(FString("Slot_01"), 0);
	SaveGameViewModel1->BindToDependencies();
	SaveGameViewModels.Add(SaveGameViewModel1);
	
	SaveGameViewModel2 = NewObject<UVM_SaveGame>(this, SaveGameViewModelClass);
	SaveGameViewModel2->SetViewModelParams(Params);
	SaveGameViewModel2->InitializeViewModel(FString("Slot_02"), 1);
	SaveGameViewModel2->BindToDependencies();
	SaveGameViewModels.Add(SaveGameViewModel2);
	
	SaveGameViewModel3 = NewObject<UVM_SaveGame>(this, SaveGameViewModelClass);
	SaveGameViewModel3->SetViewModelParams(Params);
	SaveGameViewModel3->InitializeViewModel(FString("Slot_03"), 2);
	SaveGameViewModel3->BindToDependencies();
	SaveGameViewModels.Add(SaveGameViewModel3);
}

void AMyHUD::InitOverlayUI()
{
	check(CharacterStatusViewModel)
	check(OverlayWidgetClass)
	
	OverlayWidget = CreateWidget<UMyWidgetBase>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->SetCharacterStatusViewModel(CharacterStatusViewModel);
	OverlayWidget->AddToViewport();
}

void AMyHUD::InitMenuUI()
{
	check(CharacterStatusViewModel)
	check(MenuWidgetClass)
	check(InventoryViewModel)

	MenuWidget = CreateWidget<UMyWidgetBase>(GetWorld(), MenuWidgetClass);
	MenuWidget->SetCharacterStatusViewModel(CharacterStatusViewModel);
	MenuWidget->SetInventoryViewModel(InventoryViewModel);
}

void AMyHUD::InitSaveGameUI()
{
	check(SaveGameWidgetClass)
	for (const auto& SaveGameViewModel : SaveGameViewModels)
	{
		check(SaveGameViewModel)
	}
	
	SaveGameWidget = CreateWidget<UMyWidgetBase>(GetWorld(), SaveGameWidgetClass);
	SaveGameWidget->SetSaveGameViewModel(SaveGameViewModels);
}

bool AMyHUD::OpenUI(UMyWidgetBase* UIWidget) const
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !UIWidget) return false;

	if (!UIWidget->IsInViewport())
	{
		UIWidget->AddToViewport();
		
		PC->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(UIWidget->GetCachedWidget());
		PC->SetInputMode(InputMode);
		return true;
	}
	return false;
}

bool AMyHUD::OpenMenu() const
{
	return OpenUI(MenuWidget);
}

bool AMyHUD::OpenSaveGameUI() const
{
	return OpenUI(SaveGameWidget);
}

bool AMyHUD::CloseUI(UMyWidgetBase* UIWidget) const
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !UIWidget) return false;

	if (UIWidget->IsInViewport())
	{
		UIWidget->RemoveFromParent();
		
		PC->bShowMouseCursor = false;
		const FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		return true;
	}
	return false;
}

bool AMyHUD::CloseMenu() const
{
	return CloseUI(MenuWidget);
}

bool AMyHUD::CloseSaveGameUI() const
{
	return CloseUI(SaveGameWidget);
}

void AMyHUD::ToggleUI(UMyWidgetBase* UIWidget) const
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !UIWidget) return;

	if (!UIWidget->IsInViewport())
	{
		UIWidget->AddToViewport();
		
		PC->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(UIWidget->GetCachedWidget());
		PC->SetInputMode(InputMode);
	}
	else
	{
		UIWidget->RemoveFromParent();
		
		PC->bShowMouseCursor = false;
		const FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}

void AMyHUD::ToggleMenu() const
{
	ToggleUI(MenuWidget);
}

void AMyHUD::ToggleSaveGameUI() const
{
	ToggleUI(SaveGameWidget);
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetSubsystem<UDamageNumberPoolSubsystem>()->InitializePool(DamageNumberComponentClass, DamageNumberWidgetClass, 30);
}
