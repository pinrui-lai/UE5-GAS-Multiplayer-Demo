#include "AbilitySystem/Abilities/MyDodgeAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/Tasks/AbilityTask_StartWarp.h"
#include "Game/MyGameplayTags.h"

bool UMyDodgeAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	const UAbilitySystemComponent* AvatarASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	if (!AvatarASC) return false;

	if (const FGameplayTagContainer OwnedTags = AvatarASC->GetOwnedGameplayTags(); OwnedTags.HasAny(FGameplayTagContainer(MyGameplayTags::State_Combat)))
	{
		if (OwnedTags.HasAny(FGameplayTagContainer(MyGameplayTags::Window_Combo)))
		{
			return true;
		}
	}
	else
	{
		return true;
	}
	
	return false;
}

void UMyDodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilityTask_StartWarp* StarWarpTask = UAbilityTask_StartWarp::CreateUAbilityTask_AbilityTask_StartWarp(this, WarpTargetName, WarpDistance);
	StarWarpTask->OnGetLastMovementInputTransform.AddUObject(this, &UMyDodgeAbility::OnGetLastMovementInputTransform);
	StarWarpTask->ReadyForActivation();
}

void UMyDodgeAbility::OnGetLastMovementInputTransform(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (!ForwardMontage) return;
	UAnimMontage* MontageToPlay = ForwardMontage;
	
	if (const FGameplayAbilityTargetData* Data = TargetDataHandle.Get(0))
	{
		if (Data->GetScriptStruct() == FGameplayAbilityTargetData_LocationInfo::StaticStruct())
		{
			if (const FGameplayAbilityTargetData_LocationInfo* LocationData = static_cast<const FGameplayAbilityTargetData_LocationInfo*>(Data))
			{
				const FTransform TargetDataTransform = LocationData->TargetLocation.GetTargetingTransform();
				const FRotator LastMovementInputRotation = TargetDataTransform.Rotator();

				const FVector LastMovementInputVector = LastMovementInputRotation.Vector();
				const FVector ActorForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
				
				// Calculate angle
				float DotProduct = FVector::DotProduct(ActorForwardVector, LastMovementInputVector);
				DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);
				const float AngleRadians = FMath::Acos(DotProduct);
				float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
				
				// Determine left and right directions
				const FVector CrossProduct = FVector::CrossProduct(ActorForwardVector, LastMovementInputVector);
				if (CrossProduct.Z < 0.0f)
				{
					AngleDegrees = -AngleDegrees; // Left is negative
				}
				
				// Determine which direction's montage to play
				if (-45.0f < AngleDegrees && AngleDegrees <= 45.0f)
				{
					MontageToPlay = ForwardMontage;
				}
				else if (45.0f < AngleDegrees && AngleDegrees <= 135.0f)
				{
					if (RightMontage)
					{
						MontageToPlay = RightMontage;	
					}
					
				}
				else if (135.0f < AngleDegrees || AngleDegrees < -135.0f)
				{
					if (BackwardMontage)
					{
						MontageToPlay = BackwardMontage;	
					}
					
				}
				else
				{
					if (LeftMontage)
					{
						MontageToPlay = LeftMontage;	
					}
				}
			}
		}
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("PlayMontageAndWait"), MontageToPlay);
	PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UMyDodgeAbility::OnMontageFinished);
	PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UMyDodgeAbility::OnMontageFinished);
	PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UMyDodgeAbility::OnMontageFinished);
	PlayMontageAndWaitTask->ReadyForActivation();
}

void UMyDodgeAbility::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
