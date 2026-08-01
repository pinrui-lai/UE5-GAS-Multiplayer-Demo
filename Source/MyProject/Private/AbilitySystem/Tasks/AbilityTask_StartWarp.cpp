#include "AbilitySystem/Tasks/AbilityTask_StartWarp.h"

#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/Character.h"

UAbilityTask_StartWarp::UAbilityTask_StartWarp(const FObjectInitializer& Initializer)
{
	bTickingTask = false;
}

UAbilityTask_StartWarp* UAbilityTask_StartWarp::CreateUAbilityTask_AbilityTask_StartWarp(
	UGameplayAbility* OwningAbility, const FName WarpTargetName, const float WarpDistance)
{
	UAbilityTask_StartWarp* MyTask = NewAbilityTask<UAbilityTask_StartWarp>(OwningAbility);
	MyTask->WarpTargetName = WarpTargetName;
	MyTask->WarpDistance = WarpDistance;
	return MyTask;
}

void UAbilityTask_StartWarp::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_StartWarp::Activate()
{
	Super::Activate();
	
	// Locally Controlled Character
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		const ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
		if (!Character)
		{
			EndTask();
			return;
		}
		
		UMotionWarpingComponent* MotionWarpingComponent = Character->FindComponentByClass<UMotionWarpingComponent>();
		if (!MotionWarpingComponent) return;
		
		// Prepare target data.
		FGameplayAbilityTargetDataHandle DataHandle;
		FGameplayAbilityTargetData_LocationInfo* Data = new FGameplayAbilityTargetData_LocationInfo();
		Data->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
		
		const FVector InputVector = Character->GetLastMovementInputVector();

		if (!InputVector.IsNearlyZero())
		{
			FRotator TargetRotation = InputVector.ToOrientationRotator();
			TargetRotation.Pitch = 0.f;
			TargetRotation.Roll = 0.f;

			const FVector TargetLocation = Character->GetActorLocation() + TargetRotation.Vector() * WarpDistance;
			
			// Locally controlled character update warp target directly
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(WarpTargetName, TargetLocation, TargetRotation);
			
			// Set Valid Transform for Target Data
			Data->TargetLocation.LiteralTransform = FTransform(TargetRotation, TargetLocation, FVector::ZeroVector);
		}
		// No input
		else
		{
			// Locally controlled character remove WarpTarget directly
			MotionWarpingComponent->RemoveWarpTarget(WarpTargetName);
			
			// Set Invalid Transform for Target Data
			Data->TargetLocation.LiteralTransform = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
		}
		
		// Add Target Data to Target Data Handle
		DataHandle.Add(Data);
		
		// Send target data to server.
		AbilitySystemComponent.Get()->ServerSetReplicatedTargetData(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey(),
			DataHandle,
			FGameplayTag(),
			AbilitySystemComponent.Get()->ScopedPredictionKey);
		
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnGetLastMovementInputTransform.Broadcast(DataHandle);	
		} 
	}
	// Server proxy, listening to target data.
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UAbilityTask_StartWarp::OnTargetDataReplicatedCallback);
		
		// Broadcast again if target data arrived before delegate bound.
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		
		// Wait if delegate still has not been called (Target data has not arrived).
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAbilityTask_StartWarp::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
	FGameplayTag ActivationTag) const
{
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	if (TargetDataHandle.IsValid(0))
	{
		// Server proxy Broadcast
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnGetLastMovementInputTransform.Broadcast(TargetDataHandle);	
		}
		
		if (const FGameplayAbilityTargetData* Data = TargetDataHandle.Get(0))
		{
			if (Data->GetScriptStruct() == FGameplayAbilityTargetData_LocationInfo::StaticStruct())
			{
				if (const FGameplayAbilityTargetData_LocationInfo* LocationData = static_cast<const FGameplayAbilityTargetData_LocationInfo*>(Data))
				{
					const FTransform TargetTransform = LocationData->TargetLocation.GetTargetingTransform();
					const FVector TargetLocation = TargetTransform.GetLocation();
					const FRotator TargetRotation = TargetTransform.Rotator();
					
					const ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
					if (!Character) return;
					
					UMotionWarpingComponent* MotionWarpingComponent = Character->FindComponentByClass<UMotionWarpingComponent>();
					if (!MotionWarpingComponent) return;
					
					// Invalid Transform, Server Proxy remove WarpTarget
					if (TargetRotation.IsNearlyZero() && TargetLocation.IsNearlyZero())
					{
						MotionWarpingComponent->RemoveWarpTarget(WarpTargetName);
					}
					// Valid Transform, Server Proxy update WarpTarget
					else
					{
						MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(WarpTargetName, TargetLocation, TargetRotation);	
					}
				}
			}
		}
	}
}
