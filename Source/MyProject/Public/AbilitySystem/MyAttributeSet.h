#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "MyAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class MYPROJECT_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	/** Meta Attribute */
	UPROPERTY(BlueprintReadOnly, Category="Meta Attribute")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, IncomingDamage)
	
	UPROPERTY(BlueprintReadOnly, Category="Meta Attribute")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, IncomingXP)
	/** end Meta Attribute */
	
	/** Primary Attribute */
	UPROPERTY(ReplicatedUsing=OnRep_Strength, BlueprintReadOnly, Category="Primary Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Strength)
	
	UPROPERTY(ReplicatedUsing=OnRep_Dexterity, BlueprintReadOnly, Category="Primary Attribute")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Dexterity)
	
	UPROPERTY(ReplicatedUsing=OnRep_Vitality, BlueprintReadOnly, Category="Primary Attribute")
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Vitality)
	/** end Primary Attribute */
	
	/** Secondary Attribute */
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Vital Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health)
	
	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category="Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxHealth)
	
	UPROPERTY(ReplicatedUsing=OnRep_Stamina, BlueprintReadOnly, Category="Vital Attribute")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Stamina)
	
	UPROPERTY(ReplicatedUsing=OnRep_MaxStamina, BlueprintReadOnly, Category="Vital Attribute")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxStamina)
	
	UPROPERTY(ReplicatedUsing=OnRep_Attack, BlueprintReadOnly, Category="Secondary Attribute")
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Attack)
	
	UPROPERTY(ReplicatedUsing=OnRep_ElementalAttack, BlueprintReadOnly, Category="Secondary Attribute")
	FGameplayAttributeData ElementalAttack;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, ElementalAttack)
	
	UPROPERTY(ReplicatedUsing=OnRep_Defense, BlueprintReadOnly, Category="Secondary Attribute")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Defense)
	
	UPROPERTY(ReplicatedUsing=OnRep_ElementalDefense, BlueprintReadOnly, Category="Secondary Attribute")
	FGameplayAttributeData ElementalDefense;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, ElementalDefense)
	
	UPROPERTY(ReplicatedUsing=OnRep_CriticalChance, BlueprintReadOnly, Category="Secondary Attribute")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, CriticalChance)
	/** end Secondary Attribute */
	
	/** Other Attribute */
	UPROPERTY(ReplicatedUsing=OnRep_HeadPart, BlueprintReadOnly, Category="Other Attribute")
	FGameplayAttributeData HeadPart;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, HeadPart)
	
	UPROPERTY(ReplicatedUsing=OnRep_BodyPart, BlueprintReadOnly, Category="Other Attribute")
	FGameplayAttributeData BodyPart;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, BodyPart)
	
	UPROPERTY(ReplicatedUsing=OnRep_LegPart, BlueprintReadOnly, Category="Other Attribute")
	FGameplayAttributeData LegPart;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, LegPart)
	
	UPROPERTY(ReplicatedUsing=OnRep_MoveSpeed, BlueprintReadOnly, Category="Other Attribute")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MoveSpeed)
	/** end Other Attribute*/
	
	/** Primary Attribute */
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	
	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldVitality) const;
	/** end Primary Attribute */
	
	/** Secondary Attribute */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;
	
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;
	
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldAttack) const;
	
	UFUNCTION()
	void OnRep_ElementalAttack(const FGameplayAttributeData& OldElementalAttack) const;
	
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldDefense) const;
	
	UFUNCTION()
	void OnRep_ElementalDefense(const FGameplayAttributeData& OldElementalDefense) const;
	
	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const;
	/** end Secondary Attribute */
	
	/** Other Attribute */
	UFUNCTION()
	void OnRep_HeadPart(const FGameplayAttributeData& OldHeadPart) const;
	
	UFUNCTION()
	void OnRep_BodyPart(const FGameplayAttributeData& OldBodyPart) const;
	
	UFUNCTION()
	void OnRep_LegPart(const FGameplayAttributeData& OldLegPart) const;
	
	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const;
	/** end Other Attribute */
};


