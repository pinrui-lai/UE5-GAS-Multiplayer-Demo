#pragma once

#include "CoreMinimal.h"
#include "UI/VM_MyViewModelBase.h"
#include "VM_Enemy.generated.h"

/**
 * To display Enemy's Status.
 */
UCLASS()
class MYPROJECT_API UVM_Enemy : public UVM_MyViewModelBase
{
	GENERATED_BODY()
	
public:
	void BindToDependencies();
	void OnHealthChanged(float NewValue);
	void OnMaxHealthChanged(float NewValue);
	
	void SetHealth(float InHealth);
	void SetMaxHealth(float InMaxHealth);
	void SetHealthPercent(float InHealthPercent);
	
	float GetHealth() const {return Health;};
	float GetMaxHealth() const {return MaxHealth;};
	float GetHealthPercent() const {return HealthPercent;};
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float Health = 0.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float MaxHealth = 1.f;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, Getter, Category="UI", meta=(AllowPrivateAccess))
	float HealthPercent = 0.f;
	
	void BindAttribute(FGameplayAttribute const &Attribute);
	void UpdateHealthPercent();
};
