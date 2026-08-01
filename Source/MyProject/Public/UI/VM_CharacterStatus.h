#pragma once

#include "CoreMinimal.h"
#include "VM_MyViewModelBase.h"
#include "VM_CharacterStatus.generated.h"

struct FGameplayTag;
struct FGameplayAttribute;
class UAttributeSet;
class UAbilitySystemComponent;
/**
 * To display Character's Status.
 */
UCLASS()
class MYPROJECT_API UVM_CharacterStatus : public UVM_MyViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(FGameplayTag AttributeTag) const;
	
	void BindToDependencies();
	void OnHealthChanged(float NewValue);
	void OnMaxHealthChanged(float NewValue);
	void OnStaminaChanged(float NewValue);
	void OnMaxStaminaChanged(float NewValue);
	void OnXPChanged(int NewValue);
	
	void SetLevel(int32 InLevel);
	void SetXP(int32 InXP);
	void SetXPPercent(float InXPPercent);
	void SetAttributePoints(int32 InAttributePoints);
	void SetHealth(float InHealth);
	void SetMaxHealth(float InMaxHealth);
	void SetHealthPercent(float InHealthPercent);
	void SetStamina(float InStamina);
	void SetMaxStamina(float InMaxStamina);
	void SetStaminaPercent(float InStaminaPercent);
	void SetStrength(float InStrength);
	void SetDexterity(float InDexterity);
	void SetVitality(float InVitality);
	void SetAttack(float InAttack);
	void SetElementalAttack(float InElementalAttack);
	void SetDefense(float InDefense);
	void SetElementalDefense(float InElementalDefense);
	void SetCriticalChance(float InCriticalChance);

	int32 GetLevel() const { return Level; };
	int32 GetXP() const { return XP; };
	float GetXPPercent() const { return XPPercent; };
	int32 GetAttributePoints() const { return AttributePoints; };
	float GetHealth() const {return Health;};
	float GetMaxHealth() const {return MaxHealth;};
	float GetHealthPercent() const {return HealthPercent;};
	float GetStamina() const {return Stamina;};
	float GetMaxStamina() const {return MaxStamina;};
	float GetStaminaPercent() const {return StaminaPercent;};
	float GetStrength() const { return Strength; };
	float GetDexterity() const { return Dexterity; };
	float GetVitality() const { return Vitality; };
	float GetAttack() const { return Attack; };
	float GetElementalAttack() const { return ElementalAttack; };
	float GetDefense() const { return Defense; };
	float GetElementalDefense() const { return ElementalDefense; };
	float GetCriticalChance() const { return CriticalChance; };

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	int32 Level = 0;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	int32 XP = 0;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float XPPercent = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	int32 AttributePoints = 0;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Health = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float MaxHealth = 1.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float HealthPercent = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Stamina = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float MaxStamina = 1.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float StaminaPercent = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Strength = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Dexterity = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Vitality = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Attack = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float ElementalAttack = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Defense = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float ElementalDefense = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float CriticalChance = 0.f;
	
	
	void BindAttribute(FGameplayAttribute const &Attribute);
	
	void UpdateHealthPercent();
	void UpdateStaminaPercent();
};

