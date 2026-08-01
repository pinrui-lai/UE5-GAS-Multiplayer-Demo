#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "AttributeSet.h"

#include "VM_MyViewModelBase.generated.h"

class UAbilitySystemComponent;
class APlayerController;
class APlayerState;
/**
 * Set Model(Weak Pointer) to View Model.
 */
USTRUCT(BlueprintType)
struct MYPROJECT_API FViewModelParams
{
	GENERATED_BODY()
	
	FViewModelParams(){}
	FViewModelParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS){}
	
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY()
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class MYPROJECT_API UVM_MyViewModelBase : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	void SetViewModelParams(FViewModelParams const& ViewModelParams);

	TWeakObjectPtr<APlayerController> PC;
	TWeakObjectPtr<APlayerState> PS;
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	TWeakObjectPtr<UAttributeSet> AS;
	
	// For unbind callback from delegate
	TMap<FGameplayAttribute, FDelegateHandle> BoundAttributes;
};
