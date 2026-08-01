#include "Character/MyEnemy.h"

#include "Components/WidgetComponent.h"
#include "UI/MyWidgetBase.h"

AMyEnemy::AMyEnemy()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Set ViewModel to widget component when is not on Dedicated Server.
	if (GetNetMode() == NM_DedicatedServer) return;
	UMyWidgetBase* EnemyHealthBarWidget = Cast<UMyWidgetBase>(WidgetComponent->GetUserWidgetObject());
	EnemyHealthBarWidget->SetEnemyViewModel(EnemyViewModel);
}
