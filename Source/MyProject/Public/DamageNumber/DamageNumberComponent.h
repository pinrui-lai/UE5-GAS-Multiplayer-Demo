#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageNumberComponent.generated.h"

/**
 * Set Damage Number Widget according to bIsCriticalHit (e.g., change Damage Number's color).
 */
UCLASS()
class MYPROJECT_API UDamageNumberComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeWidget(float Damage, bool bIsCriticalHit, FName HitPartName);
};
