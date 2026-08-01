#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "MyCharacterBase.generated.h"

class UMotionWarpingComponent;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class MYPROJECT_API AMyCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMyCharacterBase();
	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;
	
	/** GenericTeamAgent Interface */
	virtual FGenericTeamId GetGenericTeamId() const override;
	/** end GenericTeamAgent Interface */
	
	/** Combat Interface*/
	virtual UAnimMontage* GetDeathAnimMontage() const override{ return DeathAnimMontage; }
	virtual UNiagaraSystem* GetHitNiagara() const override { return HitNiagara; };
	virtual void Die() override;
	virtual USkeletalMeshComponent* GetWeaponMeshComponent() const override {return nullptr; }
	virtual TArray<FName> GetWeaponSocketNames() const override{ return TArray<FName>(); }
	virtual FGameplayTag GetWeaponAbilityTag() const override { return FGameplayTag(); }
	/** end Combat Interface*/
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDie();
	
protected:
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;
	void InitializeDefaultAttributes() const;
	void AddCharacterAbilities() const;
	void AddCharacterAbilitiesAndActivate() const;
	void ApplyCharacterDefaultEffects() const;
	void BindMoveSpeed() const;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY()
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent ;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Setup|Attribute")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributeEffectClass;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Setup|Attribute")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributeEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Anim")
	UAnimMontage* DeathAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Combat")
	float DeathLifeSpan = 10.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|AI")
	FGenericTeamId DefaultTeamID = 2;
	
private:
	UPROPERTY(EditDefaultsOnly, Category="Setup|Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartUpActivateAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Effect")
	TArray<TSubclassOf<UGameplayEffect>> StartUpEffects;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Niagara")
	UNiagaraSystem* HitNiagara;
};
