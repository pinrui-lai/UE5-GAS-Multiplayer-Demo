#include "AI/TargetAngleEvaluator.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"

void FTargetAngleEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FTargetAngleEvaluatorInstanceData& InstanceData = Context.GetInstanceData<FTargetAngleEvaluatorInstanceData>(*this);

	InstanceData.TimeAccumulator += DeltaTime;
	
	if (InstanceData.TimeAccumulator >= InstanceData.TickInterval)
	{
		InstanceData.TimeAccumulator -= InstanceData.TickInterval;
		
		const AActor* TargetActor = InstanceData.DetectedActor.Get();
		if (!TargetActor)
		{
			InstanceData.RelativeAngle = 0.0f;
			return;
		}
		
		const AAIController* AIController = Cast<AAIController>(Context.GetOwner());
		if (!AIController) return;

		const APawn* Pawn = AIController->GetPawn();
		if (!Pawn) return;

		const FVector PawnLocation = Pawn->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
	
		FVector ToTargetVector = TargetLocation - PawnLocation;
		ToTargetVector.Z = 0.0f;
		ToTargetVector.Normalize();
	
		FVector PawnForwardVector = Pawn->GetActorForwardVector();
		PawnForwardVector.Z = 0.0f;
		PawnForwardVector.Normalize();
	
		// Calculate angle
		float DotProduct = FVector::DotProduct(PawnForwardVector, ToTargetVector);
		DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);
		const float AngleRadians = FMath::Acos(DotProduct);
		float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	
		// Determine left and right directions
		const FVector CrossProduct = FVector::CrossProduct(PawnForwardVector, ToTargetVector);
		if (CrossProduct.Z < 0.0f)
		{
			AngleDegrees = -AngleDegrees; // Left is negative
		}
		InstanceData.RelativeAngle = AngleDegrees;
	}
}
