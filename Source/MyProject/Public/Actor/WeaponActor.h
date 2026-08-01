#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UCLASS()
class MYPROJECT_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponActor();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;
	
protected:
	virtual void BeginPlay() override;
};
