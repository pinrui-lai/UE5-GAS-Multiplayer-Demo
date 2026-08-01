#include "UI/MyMainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/MyWidgetBase.h"
#include "SaveGame/VM_SaveGame.h"

void AMyMainMenuHUD::InitSaveGameViewModel()
{
	check(SaveGameViewModelClass)

	const FViewModelParams Params(GetOwningPlayerController(), nullptr, nullptr, nullptr);
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

void AMyMainMenuHUD::InitMainMenuUI()
{
	check(MainMenuWidgetClass)
	
	MainMenuWidget = CreateWidget<UMyWidgetBase>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
}

void AMyMainMenuHUD::InitLoadScreenUI()
{
	check(LoadScreenWidgetClass)
	
	for (const auto& SaveGameViewModel : SaveGameViewModels)
	{
		check(SaveGameViewModel)
	}
	
	LoadScreenWidget = CreateWidget<UMyWidgetBase>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->SetSaveGameViewModel(SaveGameViewModels);
}

void AMyMainMenuHUD::DisplayMainMenuUI() const
{
	if (!MainMenuWidget->IsInViewport())
	{
		MainMenuWidget->AddToViewport();
	}
}

void AMyMainMenuHUD::DisplayLoadScreenUI() const
{
	if (!LoadScreenWidget->IsInViewport())
	{
		LoadScreenWidget->AddToViewport();
	}
}

void AMyMainMenuHUD::HideMainMenuUI() const
{
	if (MainMenuWidget->IsInViewport())
	{
		MainMenuWidget->RemoveFromParent();
	}
}

void AMyMainMenuHUD::HideLoadScreenUI() const
{
	if (LoadScreenWidget->IsInViewport())
	{
		LoadScreenWidget->RemoveFromParent();
	}
}

void AMyMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	InitSaveGameViewModel();
	InitMainMenuUI();
	InitLoadScreenUI();
	
	GetOwningPlayerController()->bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget->GetCachedWidget());
	GetOwningPlayerController()->SetInputMode(InputMode);
}
