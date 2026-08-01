#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEffectActor.generated.h"

class UGameplayEffect;
/**
 * Apply Effect when begin overlap.
 * Let BP side to determine which MeshComponent and CollisionComponent to use.
 */
UCLASS()
class MYPROJECT_API AMyEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyEffectActor();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Effect")
	TSubclassOf<UGameplayEffect> InstantiateEffectClass;
	
	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass);
	
};
