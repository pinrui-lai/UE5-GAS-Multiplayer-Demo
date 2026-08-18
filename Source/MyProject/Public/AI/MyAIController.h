#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

class UStateTreeComponent;

UCLASS()
class MYPROJECT_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyAIController();
	virtual FRotator GetControlRotation() const override;
	
	/** GenericTeamAgentInterface */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override { TeamID = NewTeamID; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
	/** end GenericTeamAgentInterface */
	
protected:
	virtual void BeginPlay() override;
	
	// 0 = Player, 1 = Enemy, 2 = Neutral
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|AI")
	FGenericTeamId TeamID = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStateTreeComponent> StateTreeComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

};
