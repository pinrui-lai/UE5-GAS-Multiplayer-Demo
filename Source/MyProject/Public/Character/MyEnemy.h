#pragma once

#include "CoreMinimal.h"
#include "Character/MyEnemyBase.h"
#include "MyEnemy.generated.h"

class UWidgetComponent;
/**
 * Display Health bar by WidgetComponent.
 */
UCLASS()
class MYPROJECT_API AMyEnemy : public AMyEnemyBase
{
	GENERATED_BODY()
	
public:
	AMyEnemy();
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleDefaultsOnly, category = UI)
	TObjectPtr<UWidgetComponent> WidgetComponent;
};
