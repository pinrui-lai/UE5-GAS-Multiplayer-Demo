#include "Cue/ChargeEffectCue.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

UChargeNiagaraCue::UChargeNiagaraCue()
{
}

bool UChargeNiagaraCue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if (!MyTarget) return false;

	const FVector Location = Parameters.Location;

	UNiagaraSystem* NiagaraToSpawn = nullptr;
	if (Parameters.RawMagnitude == 2)
	{
		NiagaraToSpawn = Stage2Niagara;
	}
	else if (Parameters.RawMagnitude == 3)
	{
		NiagaraToSpawn = Stage3Niagara;
	}
	else if (Parameters.RawMagnitude == 4)
	{
		NiagaraToSpawn = Stage4Niagara;
	}
	 
	if (NiagaraToSpawn && ChargeSound)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			MyTarget->GetWorld(),
			NiagaraToSpawn,
			Location,
			FRotator::ZeroRotator,
			FVector(1.f),
			true);
		
		UGameplayStatics::PlaySoundAtLocation(MyTarget->GetWorld(), ChargeSound, Location);
		
		return true;
	}
	return false;
}
