#include "Cue/HitStopCue.h"

UHitStopCue::UHitStopCue()
{
	DefaultDuration = 0.1f;
	HitStopTimeDilation = 0.01f;
}

bool UHitStopCue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const 
{
	AActor* Instigator = Parameters.Instigator.Get();
	
	if (!Instigator && !MyTarget) return false;
	
	// Hit stop by set CustomTimeDilation to very slow
	if (Instigator)
	{
		Instigator->CustomTimeDilation = HitStopTimeDilation;
	}
	
	if (bDoesTargetStop && MyTarget)
	{
		MyTarget->CustomTimeDilation = HitStopTimeDilation;
	}
	
	if (const UWorld* World = Instigator? Instigator->GetWorld() : MyTarget->GetWorld())
	{
		FTimerHandle ResetTimerHandle;
		FTimerDelegate ResetDelegate;
		
		ResetDelegate.BindLambda([Instigator, MyTarget, this]()
		{
			// Reset CustomTimeDilation to default
			if (Instigator)
			{
				Instigator->CustomTimeDilation = 1.0f;
			}
			if (bDoesTargetStop && MyTarget)
			{
				MyTarget->CustomTimeDilation = 1.0f;
			}
		});
			
		// Hit Stop Duration depending on (Motion Value / Motion Value Coefficient).
		const float Duration = (Parameters.RawMagnitude > 0.f) ? FMath::Clamp(Parameters.RawMagnitude / MotionValueCoefficient, MinHitStopDuration, MaxHitStopDuration) : DefaultDuration;
		
		// Use Global Time (World->GetTimerManager)
		World->GetTimerManager().SetTimer(ResetTimerHandle, ResetDelegate, Duration, false);
	}
	
	return true;
}
