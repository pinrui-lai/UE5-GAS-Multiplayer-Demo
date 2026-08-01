#include "UI/VM_CharacterStatus.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/MyPlayerState.h"


void UVM_CharacterStatus::UpgradeAttribute(const FGameplayTag AttributeTag) const
{
	UMyAbilitySystemComponent* MyASC = Cast<UMyAbilitySystemComponent>(ASC);
	check(MyASC);
	
	MyASC->UpgradeAttribute(AttributeTag);
}

void UVM_CharacterStatus::BindToDependencies()
{
	if (!ASC.IsValid()) return;
	if (!AS.IsValid()) return;
	if (!PS.IsValid()) return;
	
	/*
	 * Player State
	 */
	if (AMyPlayerState* MyPS = Cast<AMyPlayerState>(PS))
	{
		MyPS->OnLevelChangedDelegate.RemoveAll(this);
		MyPS->OnLevelChangedDelegate.AddUObject(this, &UVM_CharacterStatus::SetLevel);
		
		MyPS->OnXPChangedDelegate.RemoveAll(this);
		MyPS->OnXPChangedDelegate.AddUObject(this, &UVM_CharacterStatus::OnXPChanged);
		
		MyPS->OnAttributePointsChangedDelegate.RemoveAll(this);
		MyPS->OnAttributePointsChangedDelegate.AddUObject(this, &UVM_CharacterStatus::SetAttributePoints);
		
		SetLevel(MyPS->GetPlayerLevel());
		OnXPChanged(MyPS->GetXP());
		SetAttributePoints(MyPS->GetAttributePoints());
	}
	
	
	/*
	 * Attribute Set
	 */
	
	// Unbind to prevent duplicates.
	for (const TTuple<FGameplayAttribute, FDelegateHandle>& Pair : BoundAttributes)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Pair.Key).Remove(Pair.Value);
	}
	BoundAttributes.Empty();
	
	// Bind Attributes and broadcast first time.
	BindAttribute(UMyAttributeSet::GetHealthAttribute());
	BindAttribute(UMyAttributeSet::GetMaxHealthAttribute());
	BindAttribute(UMyAttributeSet::GetStaminaAttribute());
	BindAttribute(UMyAttributeSet::GetMaxStaminaAttribute());
	
	BindAttribute(UMyAttributeSet::GetStrengthAttribute());
	BindAttribute(UMyAttributeSet::GetDexterityAttribute());
	BindAttribute(UMyAttributeSet::GetVitalityAttribute());
	BindAttribute(UMyAttributeSet::GetAttackAttribute());
	BindAttribute(UMyAttributeSet::GetElementalAttackAttribute());
	BindAttribute(UMyAttributeSet::GetDefenseAttribute());
	BindAttribute(UMyAttributeSet::GetElementalDefenseAttribute());
	BindAttribute(UMyAttributeSet::GetCriticalChanceAttribute());
}

void UVM_CharacterStatus::BindAttribute(FGameplayAttribute const& Attribute)
{
	const FDelegateHandle Handle = ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda([this, Attribute](const FOnAttributeChangeData& Data)
	{
		if (Attribute == UMyAttributeSet::GetHealthAttribute()) OnHealthChanged(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetMaxHealthAttribute()) OnMaxHealthChanged(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetStaminaAttribute()) OnStaminaChanged(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetMaxStaminaAttribute()) OnMaxStaminaChanged(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetStrengthAttribute()) SetStrength(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetDexterityAttribute()) SetDexterity(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetVitalityAttribute()) SetVitality(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetAttackAttribute()) SetAttack(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetElementalAttackAttribute()) SetElementalAttack(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetDefenseAttribute()) SetDefense(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetElementalDefenseAttribute()) SetElementalDefense(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetCriticalChanceAttribute()) SetCriticalChance(Data.NewValue);
	});
	
	BoundAttributes.Add(Attribute, Handle);
	
	// Broadcast for first time.
	if (const UMyAttributeSet* MyAS = Cast<UMyAttributeSet>(AS))
	{
		if (Attribute == UMyAttributeSet::GetHealthAttribute()) OnHealthChanged(MyAS->GetHealth());
		else if (Attribute == UMyAttributeSet::GetMaxHealthAttribute()) OnMaxHealthChanged(MyAS->GetMaxHealth());
		else if (Attribute == UMyAttributeSet::GetStaminaAttribute()) OnStaminaChanged(MyAS->GetStamina());
		else if (Attribute == UMyAttributeSet::GetMaxStaminaAttribute()) OnMaxStaminaChanged(MyAS->GetMaxStamina());
		else if (Attribute == UMyAttributeSet::GetStrengthAttribute()) SetStrength(MyAS->GetStrength());
		else if (Attribute == UMyAttributeSet::GetDexterityAttribute()) SetDexterity(MyAS->GetDexterity());
		else if (Attribute == UMyAttributeSet::GetVitalityAttribute()) SetVitality(MyAS->GetVitality());
		else if (Attribute == UMyAttributeSet::GetAttackAttribute()) SetAttack(MyAS->GetAttack());
		else if (Attribute == UMyAttributeSet::GetElementalAttackAttribute()) SetElementalAttack(MyAS->GetElementalAttack());
		else if (Attribute == UMyAttributeSet::GetDefenseAttribute()) SetDefense(MyAS->GetDefense());
		else if (Attribute == UMyAttributeSet::GetElementalDefenseAttribute()) SetElementalDefense(MyAS->GetElementalDefense());
		else if (Attribute == UMyAttributeSet::GetCriticalChanceAttribute()) SetCriticalChance(MyAS->GetCriticalChance());
	}
}

void UVM_CharacterStatus::OnHealthChanged(float NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Health, NewValue))
	{
		UpdateHealthPercent();
	}
}

void UVM_CharacterStatus::OnXPChanged(int NewValue)
{
	const AMyPlayerState* MyPS = Cast<AMyPlayerState>(PS);
	check(MyPS);
	
	ULevelUpInfo* LevelUpInfo= MyPS->LevelUpInfo;
	check(LevelUpInfo);
	
	// Current Level
	const int32 PlayerLevel = LevelUpInfo->FindLevelByXP(NewValue);

	// Calculate XP Percent
	const int32 CurrentLevelUpRequirement = LevelUpInfo->LevelUpInformation[PlayerLevel].LevelUpRequirement;
	const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[PlayerLevel-1].LevelUpRequirement;

	const int32 DeltaLevelUpRequirement = CurrentLevelUpRequirement - PreviousLevelUpRequirement;
	const int32 XPForThisLevel = NewValue - PreviousLevelUpRequirement;
	
	SetXPPercent(static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement) );
}

void UVM_CharacterStatus::OnMaxHealthChanged(float NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewValue))
	{
		UpdateHealthPercent();
	}
}

void UVM_CharacterStatus::OnStaminaChanged(float NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Stamina, NewValue))
	{
		UpdateStaminaPercent();
	}
}

void UVM_CharacterStatus::OnMaxStaminaChanged(float NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxStamina, NewValue))
	{
		UpdateStaminaPercent();
	}
}

void UVM_CharacterStatus::SetLevel(int32 InLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(Level, InLevel);
}

void UVM_CharacterStatus::SetXP(int32 InXP)
{
	UE_MVVM_SET_PROPERTY_VALUE(XP, InXP);
}

void UVM_CharacterStatus::SetXPPercent(float InXPPercent)
{
	UE_MVVM_SET_PROPERTY_VALUE(XPPercent, InXPPercent);
}

void UVM_CharacterStatus::SetAttributePoints(int32 InAttributePoints)
{
	UE_MVVM_SET_PROPERTY_VALUE(AttributePoints, InAttributePoints);
}

void UVM_CharacterStatus::SetHealth(float InHealth)
{
	UE_MVVM_SET_PROPERTY_VALUE(Health, InHealth);
}

void UVM_CharacterStatus::SetMaxHealth(float InMaxHealth)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, InMaxHealth);
}

void UVM_CharacterStatus::SetHealthPercent(float InHealthPercent)
{
	UE_MVVM_SET_PROPERTY_VALUE(HealthPercent, InHealthPercent);
}

void UVM_CharacterStatus::SetStamina(float InStamina)
{
	UE_MVVM_SET_PROPERTY_VALUE(Stamina, InStamina);
}

void UVM_CharacterStatus::SetMaxStamina(float InMaxStamina)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxStamina, InMaxStamina);
}

void UVM_CharacterStatus::SetStaminaPercent(float InStaminaPercent)
{
	UE_MVVM_SET_PROPERTY_VALUE(StaminaPercent, InStaminaPercent);
}


void UVM_CharacterStatus::SetStrength(float InStrength)
{
	UE_MVVM_SET_PROPERTY_VALUE(Strength, InStrength);
}

void UVM_CharacterStatus::SetDexterity(float InDexterity)
{
	UE_MVVM_SET_PROPERTY_VALUE(Dexterity, InDexterity);
}

void UVM_CharacterStatus::SetVitality(float InVitality)
{
	UE_MVVM_SET_PROPERTY_VALUE(Vitality, InVitality);
}

void UVM_CharacterStatus::SetAttack(float InAttack)
{
	UE_MVVM_SET_PROPERTY_VALUE(Attack, InAttack);
}

void UVM_CharacterStatus::SetElementalAttack(float InElementalAttack)
{
	UE_MVVM_SET_PROPERTY_VALUE(ElementalAttack, InElementalAttack);
}

void UVM_CharacterStatus::SetDefense(float InDefense)
{
	UE_MVVM_SET_PROPERTY_VALUE(Defense, InDefense);
}

void UVM_CharacterStatus::SetElementalDefense(float InElementalDefense)
{
	UE_MVVM_SET_PROPERTY_VALUE(ElementalDefense, InElementalDefense);
}

void UVM_CharacterStatus::SetCriticalChance(float InCriticalChance)
{
	UE_MVVM_SET_PROPERTY_VALUE(CriticalChance, InCriticalChance);
}

void UVM_CharacterStatus::UpdateHealthPercent()
{
	const float NewPercent = (MaxHealth > 0) ? (Health/MaxHealth) : 0.f;
	UE_MVVM_SET_PROPERTY_VALUE(HealthPercent, NewPercent);
}

void UVM_CharacterStatus::UpdateStaminaPercent()
{
	const float NewPercent = (MaxStamina > 0) ? (Stamina/MaxStamina) : 0.f;
	UE_MVVM_SET_PROPERTY_VALUE(StaminaPercent, NewPercent);
}