#include "AbilitySystem/MyAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueManager.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/MyAbilitySystemLibrary.h"
#include "Game/MyGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Strength, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Dexterity, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Vitality, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Stamina, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Attack, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, ElementalAttack, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Defense, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, ElementalDefense, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, HeadPart, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, BodyPart, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, LegPart, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always)
}

void UMyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetMaxHealthAttribute())
	{
		const float CurrentMaxHP = GetMaxHealth();
		
		// When game start, fill up Health.
		if (CurrentMaxHP == 0.0f && NewValue > 0.0f)
		{
			SetHealth(NewValue);
		}
		// Equipment or Buff, adjust Health according to Health/MaxHealth ratio.
		else if (CurrentMaxHP > 0.0f)
		{
			const float CurrentHPPercent = GetHealth() / CurrentMaxHP;
			SetHealth(NewValue * CurrentHPPercent);
		}
	}
}

void UMyAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute()) SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	
	AActor* SourceActor = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent()->GetAvatarActor();
	
	/**
	 * Parry Event
	 */
	FGameplayEffectContextHandle ContextHandle= Data.EffectSpec.GetContext();
	if (UMyAbilitySystemLibrary::EffectContext_IsCountered(ContextHandle))
	{
		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimerForNextTick([SourceActor, Data]()
			{
				FGameplayEventData TargetPayload;
				TargetPayload.EventTag = MyGameplayTags::Event_CounterSucceed;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), MyGameplayTags::Event_CounterSucceed, TargetPayload);
				
				if (SourceActor)
				{
					FGameplayEventData SourcePayload;
					SourcePayload.EventTag = MyGameplayTags::Event_BeCountered;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceActor, MyGameplayTags::Event_BeCountered, SourcePayload);
				}
			});
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		/*
		 * Damage
		 */
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			
			ContextHandle = Data.EffectSpec.GetContext();
			
			// Dead
			if (NewHealth <= 0.f)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Data.Target.GetAvatarActor()))
				{
					CombatInterface->Die();
				}
				
				// Send XP if is Enemy
				if (IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(Data.Target.GetAvatarActor()))
				{
					if (SourceActor)
					{
						FGameplayEventData Payload;
						Payload.EventTag = MyGameplayTags::Attribute_Meta_IncomingXP;
						Payload.EventMagnitude = EnemyInterface->GetXPReward();	
						UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceActor, MyGameplayTags::Attribute_Meta_IncomingXP, Payload);
					}
				}
			}
			// Still alive
			else
			{
				FGameplayEventData Payload;
				Payload.EventTag = MyGameplayTags::Event_BeHit;
				if (!UMyAbilitySystemLibrary::EffectContext_IsBlocked(ContextHandle))
				{
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), MyGameplayTags::Event_BeHit, Payload);	
				}
			}
		}
	}
	
	/*
	 * XP
	 */
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const int32 LocalIncomingXP = GetIncomingXP();
		SetIncomingXP(0);
		
		if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(SourceActor))
		{
			const int32 CurrentLevel = PlayerInterface->GetPlayerLevel();
			const int32 CurrentXP = PlayerInterface->GetXP();

			const int32 NewLevel = PlayerInterface->FindLevelByXP(CurrentXP + LocalIncomingXP);
			const int32 NumLevelUps = NewLevel - CurrentLevel;
			if (NumLevelUps > 0)
			{
				PlayerInterface->AddToLevel(NumLevelUps);
				
				int32 AttributePointsReward = 0;
				for (int32 i = 0; i < NumLevelUps; i++)
				{
					AttributePointsReward += PlayerInterface->GetAttributePointsRewardFromInfo(CurrentLevel + i);
				}
				PlayerInterface->AddToAttributePoints(AttributePointsReward);
			}
			
			PlayerInterface->AddToXP(LocalIncomingXP);
		}
	}
}

void UMyAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Strength, OldStrength);
}

void UMyAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Dexterity, OldDexterity);
}

void UMyAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Vitality, OldVitality);
}

void UMyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Health, OldHealth);
}

void UMyAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxHealth, OldMaxHealth);
}

void UMyAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Stamina, OldStamina);
}

void UMyAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxStamina, OldMaxStamina);
}

void UMyAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Attack, OldAttack);
}

void UMyAttributeSet::OnRep_ElementalAttack(const FGameplayAttributeData& OldElementalAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, ElementalAttack, OldElementalAttack);
}

void UMyAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Defense, OldDefense);
}

void UMyAttributeSet::OnRep_ElementalDefense(const FGameplayAttributeData& OldElementalDefense) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, ElementalDefense, OldElementalDefense);
}

inline void UMyAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, CriticalChance, OldCriticalChance);
}

void UMyAttributeSet::OnRep_HeadPart(const FGameplayAttributeData& OldHeadPart) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, HeadPart, OldHeadPart);
}

void UMyAttributeSet::OnRep_BodyPart(const FGameplayAttributeData& OldBodyPart) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, BodyPart, OldBodyPart);
}

void UMyAttributeSet::OnRep_LegPart(const FGameplayAttributeData& OldLegPart) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, LegPart, OldLegPart);
}

void UMyAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MoveSpeed, OldMoveSpeed);
}
