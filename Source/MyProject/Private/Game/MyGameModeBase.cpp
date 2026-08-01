#include "Game/MyGameModeBase.h"

#include "Kismet/GameplayStatics.h"

void AMyGameModeBase::OnPlayerDied() const
{
	if (!MainMenuLevel.IsNull())
	{
		const FName LevelName = FName(*MainMenuLevel.GetAssetName());
		UGameplayStatics::OpenLevel(this, LevelName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainMenuLevel is not set in the GameMode blueprint!"));
	}
}

void AMyGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	FGenericTeamId::SetAttitudeSolver(&AMyGameModeBase::GetTeamAttitude);
}

ETeamAttitude::Type AMyGameModeBase::GetTeamAttitude(FGenericTeamId TeamA, FGenericTeamId TeamB)
{
	if (TeamA == TeamB)
	{
		return ETeamAttitude::Friendly;
	}
	
	if ((TeamA == 0 && TeamB == 1) || (TeamA == 1 && TeamB == 0))
	{
		return ETeamAttitude::Hostile;
	}
	
	return ETeamAttitude::Neutral;
}
