#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Burst.h"
#include "ChargeEffectCue.generated.h"

/**
 * Indicates which Charge Stage the Charge Attack has reached.
 */
UCLASS()
class MYPROJECT_API UChargeNiagaraCue : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UChargeNiagaraCue();
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> Stage2Niagara;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> Stage3Niagara;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> Stage4Niagara;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ChargeSound;

};
