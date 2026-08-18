#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace  MyGameplayTags
{
	/** 
	 * Attribute
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Meta_IncomingXP)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Strength)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Dexterity)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Vitality)
	
	
	/** 
	 * Event 
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Ability_Lifecycle_End)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_WeaponAbility)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_CounterSucceed)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_BeCountered)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_BeHit)
	
	
	/** 
	 * State Tree 
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTree_Event_EnterEngage)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTree_Event_LeaveEngage)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTree_Event_EnterSearch)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTree_Event_LeaveSearch)
	
	
	/**
	 * Window
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Window_Combo)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Window_PreInput)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Window_IsHitting)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Window_Warp)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Window_Counter)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Window_Block)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Window_Dodge)
	
	
	/** 
	 * State
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Idle)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Run)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_GetHit)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_BeCounter)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Parry)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Block)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_RightUpwardSlash)
	
	/** SS */ 
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_RightDownwardSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_LeftUpwardSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_RightSpinJumpSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_HiltStrike)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_Kick)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_PowerUp)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_RightSpinSlash1)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_RightSpinSlash2)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_TripleSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Sword_JumpSlash)
	/** end SS */
	
	/** GS */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_GreatSword_RightDownwardSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_GreatSword_LeftSpinSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_GreatSword_LeftSpinJumpSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_GreatSword_RightSlideSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_GreatSword_RightUpwardStrike)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_GreatSword_VerticalDownwardSlash)
	/** end GS */
	
	/** Kraken */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Kraken_ForwardSmash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Kraken_LeftSmash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Kraken_RightSmash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Kraken_TurnLeft)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Combat_Kraken_TurnRight)
	/** end Kraken */
	
	
	/** 
	 * Ability
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Common_Run)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Common_Roll)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Passive_Common_Walk)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Passive_Common_GetHit)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Passive_Common_BeCounter)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Passive_Common_Death)
	
	/** Projectile */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Common_FireProjectile)
	/** end Projectile */
	
	/** SS */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_RightDownwardSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_LeftUpwardSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_RightSpinJumpSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_HiltStrike)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_Kick)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_PowerUp)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_RightSpinSlash1)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_RightSpinSlash2)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_TripleSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_JumpSlash)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_Parry)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_Block)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Sword_RightUpwardSlash)
	/** end SS */
	
	/** GS */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_GreatSword_RightDownwardSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_GreatSword_LeftSpinSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_GreatSword_LeftSpinJumpSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_GreatSword_RightSlideSlash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_GreatSword_VerticalDownwardSlash)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_GreatSword_Parry)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_GreatSword_Block)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_GreatSword_RightUpwardStrike)
	/** end GS */
	
	/** Kraken */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Kraken_ForwardSmash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Kraken_LeftSmash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Kraken_RightSmash)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Kraken_TurnLeft)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Active_Kraken_TurnRight)
	/** end Kraken */
	
	
	/** 
	 * Set By Caller
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Ability_MotionValue)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Ability_ElementalMotionValue)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Weapon_Attack)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Weapon_ElementalAttack)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Weapon_CriticalChance)
	
	
	/**
	 * Input
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_LightAttack)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_HeavyAttack)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Run)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Walk)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Roll)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Block)
	
	
	/** 
	 * Item
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Weapon_Sword)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Weapon_GreatSword)
	
	
	/**
	 * Gameplay Cue
	 */
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Combat_DamageNumber)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Combat_HitStop)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Combat_HitNiagara)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Combat_ImpactSound)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Combat_ChargeNiagara)
}
