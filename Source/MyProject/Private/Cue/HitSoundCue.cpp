#include "Cue/HitSoundCue.h"

#include "AbilitySystem/MyAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"

bool UHitSoundCue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if (!MyTarget || !SFXAsset || !PerfectParriedSFXAsset) return false;
	
	// Default SpawnLocation and bIsParried
	FVector SpawnLocation = MyTarget->GetActorLocation();
	bool bIsCountered = false;
	
	if (Parameters.EffectContext.IsValid())
	{	
		bIsCountered = UMyAbilitySystemLibrary::EffectContext_IsCountered(Parameters.EffectContext);
		if (const FHitResult* HitResult = Parameters.EffectContext.GetHitResult())
		{
			SpawnLocation = HitResult->ImpactPoint;
		}
	}
	
	if (bIsCountered)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PerfectParriedSFXAsset, SpawnLocation);
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXAsset, SpawnLocation);
	}
	
	return true;
}
