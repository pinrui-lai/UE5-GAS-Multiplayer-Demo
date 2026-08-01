#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetDeathAnimMontage() const = 0;
	
	virtual UNiagaraSystem* GetHitNiagara() const = 0;
	
	virtual void Die() = 0;
	
	virtual USkeletalMeshComponent* GetWeaponMeshComponent() const = 0;
	virtual TArray<FName> GetWeaponSocketNames() const = 0;
	virtual FGameplayTag GetWeaponAbilityTag() const = 0;
};
