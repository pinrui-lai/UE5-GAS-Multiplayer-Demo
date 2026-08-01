#include "Cue/HitNiagaraCue.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

UHitNiagaraCue::UHitNiagaraCue()
{
}

bool UHitNiagaraCue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const 
{
	
	if (!MyTarget) return false;
	
	UNiagaraSystem* HitNiagara = nullptr;
	if (const ICombatInterface* CombatInterface = Cast<ICombatInterface>(MyTarget))
	{
		HitNiagara = CombatInterface->GetHitNiagara();	
	}
	if (!HitNiagara) return false;
	
	// Default location and rotation
	FVector SpawnLocation = MyTarget->GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	if (Parameters.EffectContext.IsValid())
	{
		if (const FHitResult* HitResult = Parameters.EffectContext.GetHitResult())
		{
			SpawnLocation = HitResult->ImpactPoint;
			SpawnRotation = UKismetMathLibrary::MakeRotFromX(HitResult->ImpactNormal);
		}
	}
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		MyTarget->GetWorld(),
		HitNiagara,
		SpawnLocation,
		SpawnRotation,
		FVector(1.f),
		true);
	
	return true;

}
