#include "Actor/MyProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Game/MyGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMyProjectile::AMyProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->InitialSpeed = ProjectileInitialSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileMaxSpeed;
}

void AMyProjectile::InitializeActor(const FGameplayEffectContextHandle& InContextHandle,
	const TSubclassOf<UGameplayEffect> InEffectClass, const float InElementalMotionValue)
{
	ContextHandle = InContextHandle;
	EffectClass = InEffectClass;
	ElementalMotionValue = InElementalMotionValue;
}

void AMyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(5.f);
}

void AMyProjectile::OnOverlapBegin(AActor* OtherActor, const FHitResult& HitResult)
{
	if (OtherActor == ContextHandle.GetEffectCauser()) return;
	if (AlreadyHitActors.Contains(OtherActor)) return;
	if (!HasAuthority()) return;
	if (!ContextHandle.IsValid()) return;
	if (!EffectClass) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!TargetASC) return;

	// Make sure don't hit same actor more than once
	AlreadyHitActors.Add(OtherActor);
	
	FGameplayEffectContextHandle CurrentContextHandle = ContextHandle.Duplicate();
	CurrentContextHandle.AddHitResult(HitResult);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectClass, 1, CurrentContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, MyGameplayTags::SetByCaller_Ability_ElementalMotionValue, ElementalMotionValue);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}
