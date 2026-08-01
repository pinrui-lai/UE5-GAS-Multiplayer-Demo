#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BossInterface.generated.h"

class UVM_Enemy;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, NotBlueprintable)
class UBossInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IBossInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual UVM_Enemy* GetBossViewModel() = 0;
};
