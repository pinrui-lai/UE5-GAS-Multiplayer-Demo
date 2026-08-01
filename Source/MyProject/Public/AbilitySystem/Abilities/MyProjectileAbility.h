#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MyProjectileAbility.generated.h"

class AMyProjectile;
/**
 * Cast projectile from weapon socket.
 */
UCLASS()
class MYPROJECT_API UMyProjectileAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UMyProjectileAbility();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup|Effect")
	TSubclassOf<UGameplayEffect> AbilityEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Motion Value")
	float ElementalMotionValue;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Projectile")
	TSubclassOf<AMyProjectile> ProjectileActorClass;
	
};
