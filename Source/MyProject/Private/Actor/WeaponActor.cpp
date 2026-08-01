#include "Actor/WeaponActor.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	RootComponent = WeaponMeshComponent;
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}