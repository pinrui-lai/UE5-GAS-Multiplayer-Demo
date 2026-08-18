#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "TargetAngleEvaluator.generated.h"

/**
 * Evaluate the angle between DetectedActor and ContextActor
 */
USTRUCT()
struct FTargetAngleEvaluatorInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Setup")
	float TickInterval = 0.2f;
	
	float TimeAccumulator = 0.0f;
	
	// Will be set by AIPerceptionComponent
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AActor> DetectedActor = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Output")
	float RelativeAngle = 0.0f;
};

USTRUCT(meta = (DisplayName = "Get Target Angle"))
struct FTargetAngleEvaluator : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()
	
	virtual const UStruct* GetInstanceDataType() const override { return FTargetAngleEvaluatorInstanceData::StaticStruct();}
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
