#include "Character/MyCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"

AMyCharacterBase::AMyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* AMyCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AMyCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

FGenericTeamId AMyCharacterBase::GetGenericTeamId() const
{
	if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(GetController()))
	{
		return TeamAgent->GetGenericTeamId();
	}
	
	return DefaultTeamID;
}

void AMyCharacterBase::Die()
{
	MulticastHandleDie();
}

void AMyCharacterBase::MulticastHandleDie_Implementation()
{
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCharacterMovement()->MaxWalkSpeed = 0;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}

void AMyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void AMyCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> EffectClass, const float Level) const
{
	check(GetAbilitySystemComponent());
	check(EffectClass);
	
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void AMyCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributeEffectClass, 1);
	ApplyEffectToSelf(DefaultSecondaryAttributeEffectClass, 1);
}

void AMyCharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority()) return;
	UMyAbilitySystemComponent* MyASC = CastChecked<UMyAbilitySystemComponent>(AbilitySystemComponent);
	MyASC->AddCharacterAbilities(StartUpAbilities);
}

void AMyCharacterBase::AddCharacterAbilitiesAndActivate() const
{
	if (!HasAuthority()) return;
	UMyAbilitySystemComponent* MyASC = CastChecked<UMyAbilitySystemComponent>(AbilitySystemComponent);
	MyASC->AddCharacterAbilitiesAndActivate(StartUpActivateAbilities);
}

void AMyCharacterBase::ApplyCharacterDefaultEffects() const
{
	for (auto& Effect : StartUpEffects)
	{
		ApplyEffectToSelf(Effect, 1);
	}
}

void AMyCharacterBase::BindMoveSpeed() const
{
	UMyAbilitySystemComponent* MyASC = Cast<UMyAbilitySystemComponent>(AbilitySystemComponent);
	MyASC->GetGameplayAttributeValueChangeDelegate(UMyAttributeSet::GetMoveSpeedAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
	{
		GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
	});
	// Set for first time
	GetCharacterMovement()->MaxWalkSpeed = CastChecked<UMyAttributeSet>(GetAttributeSet())->GetMoveSpeed();
}
