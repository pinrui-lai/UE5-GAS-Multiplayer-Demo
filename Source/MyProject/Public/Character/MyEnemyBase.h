#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "MyEnemyBase.generated.h"

class UVM_Enemy;
/**
 * Create View Model, let child class determine how to display View Model's data (e.g., Health).
 */
UCLASS()
class MYPROJECT_API AMyEnemyBase : public AMyCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AMyEnemyBase();
	virtual void BeginPlay() override;
	
	/** Combat Interface*/
	virtual void Die() override;
	virtual USkeletalMeshComponent* GetWeaponMeshComponent() const override { return GetMesh();}
	virtual TArray<FName> GetWeaponSocketNames() const override { return WeaponSocketNames; }
	/** end Combat Interface*/
	
	/** Enemy Interface*/
	virtual int32 GetXPReward() override { return XPReward; }
	/** end Enemy Interface*/
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Combat")
	TArray<FName> WeaponSocketNames;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|XP")
	int32 XPReward ;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|UI")
	TSubclassOf<UVM_Enemy> EnemyViewModelClass;
	
	UPROPERTY()
	TObjectPtr<UVM_Enemy> EnemyViewModel;
	
};
