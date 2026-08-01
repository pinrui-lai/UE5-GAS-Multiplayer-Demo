#include "Cue/DamageNumberCue.h"

#include "AbilitySystem/MyAbilitySystemLibrary.h"
#include "DamageNumber/DamageNumberPoolSubsystem.h"
#include "DamageNumber/DamageNumberComponent.h"

bool UDamageNumberCue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const float DamageAmount = Parameters.RawMagnitude;
	const bool bIsCriticalHit = UMyAbilitySystemLibrary::EffectContext_IsCriticalHit(Parameters.EffectContext);
	FName HitPartName;
	
	// Default PopupLocation
	FVector PopupLocation = MyTarget->GetActorLocation();
	
	if (Parameters.EffectContext.IsValid())
	{
		if (const FHitResult* HitResult = Parameters.EffectContext.GetHitResult())
		{
			PopupLocation = HitResult->ImpactPoint;
			
			// Hit part Physic Material
			if (const UPhysicalMaterial* HitPartPhysMat = HitResult->PhysMaterial.Get())
			{
				if (HitPartPhysMat->GetName().Contains(TEXT("Head")))
				{
					HitPartName = TEXT("Head");
				}
				else if (HitPartPhysMat->GetName().Contains(TEXT("Body")))
				{
					HitPartName = TEXT("Body");
				}
				else if (HitPartPhysMat->GetName().Contains(TEXT("Leg")))
				{
					HitPartName = TEXT("Leg");
				}
			}
		}
	}

	const float JitterX = FMath::RandRange(-10.0f, 10.0f);
	const float JitterY = FMath::RandRange(-15.0f, 15.0f);
	const float JitterZ = FMath::RandRange(-10.0f, 10.0f);
	const FVector RandomOffset(JitterX, JitterY, JitterZ);
	PopupLocation += RandomOffset;
	
	if (UDamageNumberPoolSubsystem* DamageNumberPoolSubsystem = MyTarget->GetWorld()->GetSubsystem<UDamageNumberPoolSubsystem>())
	{
		UDamageNumberComponent* WidgetComponent = DamageNumberPoolSubsystem->GetWidgetComponentFromPool();
		
		// Will call ReturnToPool() in the end
		WidgetComponent->InitializeWidget(DamageAmount, bIsCriticalHit, HitPartName);
		
		WidgetComponent->SetWorldLocation(PopupLocation);
		WidgetComponent->SetHiddenInGame(false);
	}
	return true;
}
