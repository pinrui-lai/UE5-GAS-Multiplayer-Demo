#include "UI/VM_MyViewModelBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

void UVM_MyViewModelBase::SetViewModelParams(FViewModelParams const& ViewModelParams)
{
	PC = ViewModelParams.PlayerController;
	PS = ViewModelParams.PlayerState;
	ASC = ViewModelParams.AbilitySystemComponent;
	AS = ViewModelParams.AttributeSet;
}
