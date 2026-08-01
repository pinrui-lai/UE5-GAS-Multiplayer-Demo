#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "MyProjectile.generated.h"

class UGameplayEffect;
class UProjectileMovementComponent;
/**
 * Apply Effect when hit.
 */
UCLASS()
class MYPROJECT_API AMyProjectile : public AActor
{
	GENERATED_BODY()

public:
	AMyProjectile();

	void InitializeActor(const FGameplayEffectContextHandle& InContextHandle, const TSubclassOf<UGameplayEffect> InEffectClass, const float InElementalMotionValue);
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(AActor* OtherActor, const FHitResult& HitResult);
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Projectile")
	float ProjectileInitialSpeed = 900.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Projectile")
	float ProjectileMaxSpeed = 900.f;

private:
	FGameplayEffectContextHandle ContextHandle;
	TSubclassOf<UGameplayEffect> EffectClass;
	float ElementalMotionValue{1};
	
	TArray<TObjectPtr<AActor>> AlreadyHitActors;
};
