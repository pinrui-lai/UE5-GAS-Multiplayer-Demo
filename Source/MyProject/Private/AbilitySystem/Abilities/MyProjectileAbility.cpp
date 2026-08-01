#include "AbilitySystem/Abilities/MyProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/MyProjectile.h"
#include "Interaction/CombatInterface.h"

UMyProjectileAbility::UMyProjectileAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UMyProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface) return;
	
	const USkeletalMeshComponent* WeaponMeshComponent = CombatInterface->GetWeaponMeshComponent();
	if (!WeaponMeshComponent) return;
	
	TArray<FName> WeaponSocketNames = CombatInterface->GetWeaponSocketNames();
	if ( WeaponSocketNames.Num() <= 0) return;

	const FVector SpawnLocation = WeaponMeshComponent->GetSocketLocation(WeaponSocketNames[0]);
	const FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorRotation();
			
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
			
	AMyProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMyProjectile>(
		ProjectileActorClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			
	// Make and Set Effect, then set to projectile
	if (const UAbilitySystemComponent* AvatarASC = GetAbilitySystemComponentFromActorInfo())
	{
		const FGameplayEffectContextHandle ContextHandle = AvatarASC->MakeEffectContext();
		Projectile->InitializeActor(ContextHandle, AbilityEffectClass, ElementalMotionValue);	
		Projectile->FinishSpawning(SpawnTransform);
	}
}
