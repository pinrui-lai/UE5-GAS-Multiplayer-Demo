#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPickUpActor.generated.h"

class UItemDataAsset;
/**
 * Add to Player's Inventory When begin overlap.
 * Let BP side to determine which MeshComponent and CollisionComponent to use.
 */
UCLASS()
class MYPROJECT_API AMyPickUpActor : public AActor
{
	GENERATED_BODY()

public:
	AMyPickUpActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Item")
	FPrimaryAssetId ItemPrimaryAssetId;
	
	// The distance of Floating Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Floating Movement")
	float FloatingAmplitude = 50.0f;

	// The speed of Floating Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|Floating Movement")
	float FloatingFrequency = 2.0f;
	
	float RunningTime = 0.0f;
	
	FVector InitialLocation;
};
