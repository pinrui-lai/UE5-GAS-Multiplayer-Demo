#include "Actor/MyPickUpActor.h"

#include "Interaction/PlayerInterface.h"

AMyPickUpActor::AMyPickUpActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyPickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RunningTime += DeltaTime;

	// Calculate Sine Period according to Floating Frequency, then reset RunningTime if larger than Sine Period 
	const float SinePeriod = 2 * PI / FloatingFrequency;
	if (RunningTime > SinePeriod)
	{
		RunningTime = 0.f;
	}
	
	const float Sine = FloatingAmplitude * FMath::Sin(RunningTime * FloatingFrequency);
	SetActorLocation(InitialLocation + FVector(0.f, 0.f, Sine));
}

void AMyPickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLocation = GetActorLocation();
}

void AMyPickUpActor::OnBeginOverlap(AActor* TargetActor)
{
	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(TargetActor);
	if (!PlayerInterface) return;
	
	if (!ItemPrimaryAssetId.IsValid()) return;
	
	PlayerInterface->AddItemToInventory(ItemPrimaryAssetId, 1);
	Destroy();
}



