#include "UI/VM_Enemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"

void UVM_Enemy::BindToDependencies()
{
	if (!ASC.IsValid()) return;
	if (!AS.IsValid()) return;
	
	// Unbind to prevent duplicates.
	for (TTuple<FGameplayAttribute, FDelegateHandle>& Pair : BoundAttributes)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Pair.Key).Remove(Pair.Value);
	}
	BoundAttributes.Empty();
	
	// Bind Attributes and broadcast first time.
	BindAttribute(UMyAttributeSet::GetHealthAttribute());
	BindAttribute(UMyAttributeSet::GetMaxHealthAttribute());
}

void UVM_Enemy::BindAttribute(FGameplayAttribute const& Attribute)
{
	const FDelegateHandle Handle = ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda([this, Attribute](const FOnAttributeChangeData& Data)
	{
		if (Attribute == UMyAttributeSet::GetHealthAttribute())	OnHealthChanged(Data.NewValue);
		else if (Attribute == UMyAttributeSet::GetMaxHealthAttribute()) OnMaxHealthChanged(Data.NewValue);
	});
	
	BoundAttributes.Add(Attribute, Handle);
	
	// Broadcast for first time.
	if (const UMyAttributeSet* MyAS = Cast<UMyAttributeSet>(AS))
	{
		if (Attribute == UMyAttributeSet::GetHealthAttribute()) OnHealthChanged(MyAS->GetHealth());
		else if (Attribute == UMyAttributeSet::GetMaxHealthAttribute()) OnMaxHealthChanged(MyAS->GetMaxHealth());
	}
}

void UVM_Enemy::OnHealthChanged(float NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(Health, NewValue))
	{
		UpdateHealthPercent();
	}
}

void UVM_Enemy::OnMaxHealthChanged(float NewValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, NewValue))
	{
		UpdateHealthPercent();
	}
}

void UVM_Enemy::SetHealth(float InHealth)
{
	UE_MVVM_SET_PROPERTY_VALUE(Health, InHealth);
}

void UVM_Enemy::SetMaxHealth(float InMaxHealth)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, InMaxHealth);
}

void UVM_Enemy::SetHealthPercent(float InHealthPercent)
{
	UE_MVVM_SET_PROPERTY_VALUE(HealthPercent, InHealthPercent);
}



void UVM_Enemy::UpdateHealthPercent()
{
	const float NewPercent = (MaxHealth > 0) ? (Health / MaxHealth) : 0.f;
	UE_MVVM_SET_PROPERTY_VALUE(HealthPercent, NewPercent);
}
