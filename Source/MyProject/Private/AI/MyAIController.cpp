#include "AI/MyAIController.h"

#include "Components/StateTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>("StateTreeComponent");
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
}

FRotator AMyAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator::ZeroRotator;
	}
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
}


