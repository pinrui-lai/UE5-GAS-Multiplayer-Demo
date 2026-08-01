#include "AbilitySystem/Tasks//AbilityTask_HitTrace.h"

#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UAbilityTask_HitTrace::UAbilityTask_HitTrace(const FObjectInitializer& Initializer)
: Super(Initializer)
{
	bTickingTask = true;
	bIsFirstFrame = true;
}

UAbilityTask_HitTrace* UAbilityTask_HitTrace::CreateUHitTraceAbilityTask(UGameplayAbility* OwningAbility,USkeletalMeshComponent* WeaponMesh, TArray<FName> SocketNames, ECollisionChannel CollisionChannel, float SphereRadius)
{
	UAbilityTask_HitTrace* MyTask = NewAbilityTask<UAbilityTask_HitTrace>(OwningAbility);
	MyTask->WeaponMechComponent = WeaponMesh;
	MyTask->SocketNames = SocketNames;
	MyTask->CollisionChannel = CollisionChannel;
	MyTask->SphereRadius = SphereRadius;
	return MyTask;
}

void UAbilityTask_HitTrace::Activate()
{
	Super::Activate();

	// Locally controlled actor, do trace in TaskTick and send target data to server when bHit==true.
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		if (!WeaponMechComponent)
		{
			EndTask();
			return;
		}
	
		PreviousSocketLocations.Empty();
		AlreadyHitActors.Empty();
		
		// Get first frame socket locations
		for (const FName SocketName : SocketNames)
		{
			PreviousSocketLocations.Add(WeaponMechComponent->GetSocketLocation(SocketName));
		}
	}
	// Server proxy, listening to target data.
	else
	{
		bTickingTask = false;
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UAbilityTask_HitTrace::OnTargetDataReplicatedCallback);
		
		// Broadcast again if target data arrived before delegate bound.
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		
		// Wait if delegate still has not been called (Target data has not arrived).
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
	

}

void UAbilityTask_HitTrace::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	
	// Only locally controlled.
	if (!Ability->GetCurrentActorInfo()->IsLocallyControlled()) return;
	if (!WeaponMechComponent) return;
		
	// Do not trace at first frame.
	if (bIsFirstFrame)
	{
		bIsFirstFrame = false;
		return;
	}

	// Trace params.
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActor());

	// Every socket do trace between "current(this frame) socket location" and "previous(last frame) socket location".
	for (int32 i = 0; i < SocketNames.Num(); i++)
	{
		// Get socket location this frame.
		const FVector CurrentSocketLocation = WeaponMechComponent->GetSocketLocation(SocketNames[i]);
		
		bool bHit;
		
		if (SphereRadius > 0.f)
		{
			// Sphere trace.
			bHit = UKismetSystemLibrary::SphereTraceMulti(
				GetWorld(),
				PreviousSocketLocations[i],
				CurrentSocketLocation,
				SphereRadius,
				UEngineTypes::ConvertToTraceType(CollisionChannel),
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResults,
				true
				);
		}
		else
		{
			// Line trace.
			bHit = UKismetSystemLibrary::LineTraceMulti(
				GetWorld(),
				PreviousSocketLocations[i],
				CurrentSocketLocation,
				UEngineTypes::ConvertToTraceType(CollisionChannel),
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResults,
				true
				);
		}
	
		// If Hit, Send Target Data and broadcast HitResult.
		if (bHit)
		{
			for (const FHitResult& HitResult : HitResults)
			{
				AActor* HitActor = HitResult.GetActor();
				// If HitActor still has not been hit by this slash.
				if (HitActor && !AlreadyHitActors.Contains(HitActor))
				{
					AlreadyHitActors.Add(HitActor);
					
					// Prepare target data.
					FGameplayAbilityTargetDataHandle DataHandle;
					FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
					Data->HitResult = HitResult;
					DataHandle.Add(Data);
					
					// Send target data to server.
					AbilitySystemComponent.Get()->ServerSetReplicatedTargetData(
						GetAbilitySpecHandle(),
						GetActivationPredictionKey(),
						DataHandle,
						FGameplayTag(),
						AbilitySystemComponent.Get()->ScopedPredictionKey);
					
					// Locally controlled actor broadcast directly.
					if (ShouldBroadcastAbilityTaskDelegates())
					{
						OnTraceHit.Broadcast(DataHandle);
					}
				}
			}
		}
		// Update this socket's previous location to current location.
		PreviousSocketLocations[i] = CurrentSocketLocation;
	}
}

void UAbilityTask_HitTrace::OnDestroy(bool bInOwnerFinished)
{
	PreviousSocketLocations.Empty();
	AlreadyHitActors.Empty();
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_HitTrace::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag) const
{
	AbilitySystemComponent.Get()->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	
	// Broadcast on server.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTraceHit.Broadcast(DataHandle);
	}
}
