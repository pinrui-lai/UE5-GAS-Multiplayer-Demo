#include "Character/MyEnemyBase.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "Game/MyGameplayTags.h"
#include "UI/VM_Enemy.h"

AMyEnemyBase::AMyEnemyBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UMyAttributeSet>("AttributeSet");
}

void AMyEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeDefaultAttributes();
	AddCharacterAbilities();
	AddCharacterAbilitiesAndActivate();
	
	// Create and set ViewModel when is not on Dedicated Server.
	if (GetNetMode() == NM_DedicatedServer) return;
	EnemyViewModel = NewObject<UVM_Enemy>(this, EnemyViewModelClass);
	EnemyViewModel->SetViewModelParams(FViewModelParams(nullptr, nullptr, AbilitySystemComponent, AttributeSet));
	EnemyViewModel->BindToDependencies();
}

void AMyEnemyBase::Die()
{
	Super::Die();
	
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(MyGameplayTags::Ability_Passive_Common_Death);
	AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
	SetLifeSpan(DeathLifeSpan);
}
