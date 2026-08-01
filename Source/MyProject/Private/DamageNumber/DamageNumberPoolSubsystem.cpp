#include "DamageNumber/DamageNumberPoolSubsystem.h"
#include "DamageNumber/DamageNumberComponent.h"

void UDamageNumberPoolSubsystem::InitializePool(const TSubclassOf<UDamageNumberComponent> DamageNumberComponentClass, const TSubclassOf<UUserWidget> WidgetClass,
                                                const int32 PoolSize)
{
	AActor* WorldSettingsActor = Cast<AActor>(GetWorld()->GetWorldSettings());
	if (!WorldSettingsActor) return;
	
	for (int i = 0; i < PoolSize; i++)
	{
		UDamageNumberComponent* DamageNumberComponent = NewObject<UDamageNumberComponent>(WorldSettingsActor, DamageNumberComponentClass);
		DamageNumberComponent->SetWidgetClass(WidgetClass);
		DamageNumberComponent->SetWidgetSpace(EWidgetSpace::Screen);
		DamageNumberComponent->SetDrawAtDesiredSize(true);
		DamageNumberComponent->RegisterComponent();
		DamageNumberComponent->SetHiddenInGame(true);
		
		AvailablePool.Add(DamageNumberComponent);
	}
}

UDamageNumberComponent* UDamageNumberPoolSubsystem::GetWidgetComponentFromPool()
{
	UDamageNumberComponent* TargetComponent;
	if (AvailablePool.Num() > 0)
	{
		TargetComponent = AvailablePool.Pop();
	}
	else
	{
		// Take oldest one from InUsePool
		TargetComponent = InUsePool[0];
		InUsePool.RemoveAt(0);
	}
	
	if (TargetComponent)
	{
		// Add to last
		InUsePool.Add(TargetComponent);
		TargetComponent->SetHiddenInGame(false);
	}
	return TargetComponent;
}

void UDamageNumberPoolSubsystem::ReturnToPool(UDamageNumberComponent* DamageNumberComponent)
{
	if (DamageNumberComponent)
	{
		DamageNumberComponent->SetHiddenInGame(true);
		InUsePool.Remove(DamageNumberComponent);
		AvailablePool.AddUnique(DamageNumberComponent);
	}
}
