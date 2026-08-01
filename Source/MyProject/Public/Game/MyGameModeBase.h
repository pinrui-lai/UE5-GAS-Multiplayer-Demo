#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void OnPlayerDied() const;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	static ETeamAttitude::Type GetTeamAttitude(FGenericTeamId TeamA, FGenericTeamId TeamB);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category= "Setup|Level")
	TSoftObjectPtr<UWorld> MainMenuLevel;	
};
