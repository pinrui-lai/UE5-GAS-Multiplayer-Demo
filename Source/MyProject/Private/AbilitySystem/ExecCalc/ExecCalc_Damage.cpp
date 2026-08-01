#include "AbilitySystem/ExecCalc//ExecCalc_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/MyAbilitySystemLibrary.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "Game/MyGameplayTags.h"

struct DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ElementalDefense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HeadPart);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BodyPart);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LegPart);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ElementalAttack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance);
	
	
	DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, ElementalDefense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, HeadPart, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, BodyPart, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, LegPart, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, ElementalAttack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, CriticalChance, Source, false);
	}
};

namespace
{
	const DamageStatics& GetDamageStatics()
	{
		static DamageStatics DamageStatics;
		return DamageStatics;
	}	
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ElementalDefenseDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().AttackDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ElementalAttackDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().HeadPartDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BodyPartDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().LegPartDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Get information
	// const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	// const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	//
	// AActor* SourceActor = SourceASC? SourceASC->GetAvatarActor(): nullptr;
	// AActor* TargetActor = TargetASC? TargetASC->GetAvatarActor(): nullptr;
	
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle ContextHandle = EffectSpec.GetContext();
	
	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	float HitPartDamageMultiplier = 1; // Default Hit Part Damage Multiplier
	if (const FHitResult* HitResult = ContextHandle.GetHitResult(); HitResult && HitResult->PhysMaterial.IsValid())
	{
		if (const UPhysicalMaterial* HitPartPhysMat = HitResult->PhysMaterial.Get())
		{
			if (HitPartPhysMat->GetName().Contains(TEXT("Head")))
			{
				float HeadPart = 0;
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().HeadPartDef, EvaluateParameters, HeadPart);
				HitPartDamageMultiplier = HeadPart;
			}
			else if (HitPartPhysMat->GetName().Contains(TEXT("Body")))
			{
				float BodyPart = 0;
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BodyPartDef, EvaluateParameters, BodyPart);
				HitPartDamageMultiplier = BodyPart;
			}
			else if (HitPartPhysMat->GetName().Contains(TEXT("Leg")))
			{
				float LegPart = 0;
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().LegPartDef, EvaluateParameters, LegPart);
				HitPartDamageMultiplier = LegPart;
			}
		}
	}
	
	// Physical Damage Calculation
	// (Source Attack * Motion Value - Target Defense)
	// *1.5 if is a Critical Hit
	const float MotionValue = EffectSpec.GetSetByCallerMagnitude(MyGameplayTags::SetByCaller_Ability_MotionValue);
	float SourceAttack = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().AttackDef, EvaluateParameters, SourceAttack);
	float PhysicalDamage = MotionValue * SourceAttack * HitPartDamageMultiplier;
	
	float TargetDefense = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().DefenseDef, EvaluateParameters, TargetDefense);
	PhysicalDamage -= TargetDefense;
	PhysicalDamage = FMath::Clamp(PhysicalDamage, 0, PhysicalDamage);
	
	if (PhysicalDamage > 0)
	{
		float CriticalChance = 0;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalChanceDef, EvaluateParameters, CriticalChance);
		const bool bIsCriticalHit = FMath::RandRange(0, 100) < CriticalChance;
		UMyAbilitySystemLibrary::EffectContext_SetIsCriticalHit(ContextHandle, bIsCriticalHit);
		if (bIsCriticalHit)
		{
			PhysicalDamage *= 1.5;
		}	
	}
	
	
	// Elemental Damage Calculation
	// Source Elemental Attack * Source Elemental Motion Value - Target Elemental Defense
	// Won't be affected by Critical Hit
	const float ElementalMotionValue = EffectSpec.GetSetByCallerMagnitude(MyGameplayTags::SetByCaller_Ability_ElementalMotionValue);
	float SourceElementalAttack = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ElementalAttackDef, EvaluateParameters, SourceElementalAttack);
	float ElementalDamage = ElementalMotionValue * SourceElementalAttack;

	float TargetElementalDefense = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ElementalDefenseDef, EvaluateParameters, TargetElementalDefense);
	ElementalDamage -= TargetElementalDefense;
	ElementalDamage = FMath::Clamp(ElementalDamage, 0, ElementalDamage);
	
	// Add the two damages
	float Damage = PhysicalDamage + ElementalDamage;
	
	// If Perfect Parry
	if (TargetTags->HasAny(FGameplayTagContainer(MyGameplayTags::Window_Block)))
	{
		UMyAbilitySystemLibrary::EffectContext_SetIsBlocked(ContextHandle, true);
		Damage *= 0.5;
	}
	
	// If Perfect Parry
	if (TargetTags->HasAny(FGameplayTagContainer(MyGameplayTags::Window_Counter)))
	{
		UMyAbilitySystemLibrary::EffectContext_SetIsCountered(ContextHandle, true);
		Damage = 0;
	}
	
	// Add calculation result to output
	const FGameplayModifierEvaluatedData ModifierEvaluatedData(UMyAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(ModifierEvaluatedData);
}
