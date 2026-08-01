#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/Data/ItemDataAsset.h"
#include "WeaponDataAsset.generated.h"

class UGameplayAbility;
class AWeaponActor;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UWeaponDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly)
	FGameplayTag AbilityTag;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly)
	float Attack;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly)
	float ElementalAttack;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly)
	float CriticalChance;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly)
	TSoftClassPtr<AWeaponActor> WeaponActorClass;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly)
	FGameplayTag WeaponTypeTag;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadOnly)
	TArray<FName> WeaponSockets;
	
	UPROPERTY()
	bool bIsEquipping;
};
