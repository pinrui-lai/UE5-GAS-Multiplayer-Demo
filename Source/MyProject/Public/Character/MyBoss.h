#pragma once

#include "CoreMinimal.h"
#include "Character/MyEnemyBase.h"
#include "Interaction/BossInterface.h"
#include "MyBoss.generated.h"

/**
 * Implement Boss Interface to let Player display Boss Health bar at the bottom of Player's HUD.
 */
UCLASS()
class MYPROJECT_API AMyBoss : public AMyEnemyBase, public IBossInterface
{
	GENERATED_BODY()
	
public:
	virtual UVM_Enemy* GetBossViewModel() override { return EnemyViewModel; } 
};
