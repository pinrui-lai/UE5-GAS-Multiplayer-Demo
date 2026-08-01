#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DamageNumberPoolSubsystem.generated.h"

class UDamageNumberComponent;
class UWidgetComponent;
/**
 * Object Pooling to save the overhead of frequently creating and deleting components.
 */
UCLASS()
class MYPROJECT_API UDamageNumberPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Combat UI")
	void InitializePool(TSubclassOf<UDamageNumberComponent> DamageNumberComponentClass, TSubclassOf<UUserWidget> WidgetClass, int32 PoolSize);
	
	UFUNCTION(BlueprintCallable, Category = "Combat UI")
	UDamageNumberComponent* GetWidgetComponentFromPool();
	
	UFUNCTION(BlueprintCallable, Category = "Combat UI")
	void ReturnToPool(UDamageNumberComponent* WidgetComponent);
	
protected:
	UPROPERTY()
	TArray<UDamageNumberComponent*> AvailablePool;
	
	UPROPERTY()
	TArray<UDamageNumberComponent*> InUsePool;
	
};
