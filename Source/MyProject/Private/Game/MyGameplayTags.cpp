#include "Game/MyGameplayTags.h"

namespace MyGameplayTags
{
	/** 
	 * Attribute
	 */
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Meta_IncomingXP, "Attribute.Meta.IncomingXP")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Strength, "Attribute.Primary.Strength")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Dexterity, "Attribute.Primary.Dexterity")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Vitality, "Attribute.Primary.Vitality")

	
	/** 
	 * Event 
	*/
	UE_DEFINE_GAMEPLAY_TAG(Event_Ability_Lifecycle_End, "Event.Ability.Lifecycle.End")
	UE_DEFINE_GAMEPLAY_TAG(Event_WeaponAbility, "Event.WeaponAbility")
	UE_DEFINE_GAMEPLAY_TAG(Event_CounterSucceed, "Event.CounterSucceed")
	UE_DEFINE_GAMEPLAY_TAG(Event_BeCountered, "Event.BeCountered")
	UE_DEFINE_GAMEPLAY_TAG(Event_BeHit, "Event.BeHit")
	
	
	/**
	 * Window
	 */
	UE_DEFINE_GAMEPLAY_TAG(Window_Combo, "Window.Combo")
	UE_DEFINE_GAMEPLAY_TAG(Window_PreInput, "Window.PreInput")
	UE_DEFINE_GAMEPLAY_TAG(Window_IsHitting, "Window.IsHitting")
	UE_DEFINE_GAMEPLAY_TAG(Window_Counter, "Window.Counter")
	UE_DEFINE_GAMEPLAY_TAG(Window_Block, "Window.Block")
	UE_DEFINE_GAMEPLAY_TAG(Window_Dodge, "Window.Dodge")
	
	
	/** 
	 * State
	 */
	UE_DEFINE_GAMEPLAY_TAG(State_Idle, "State.Idle")
	UE_DEFINE_GAMEPLAY_TAG(State_Run, "State.Run")
	UE_DEFINE_GAMEPLAY_TAG(State_GetHit, "State.GetHit")
	UE_DEFINE_GAMEPLAY_TAG(State_BeCounter, "State.BeCounter")
	
	UE_DEFINE_GAMEPLAY_TAG(State_Combat, "State.Combat")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Parry, "State.Combat.Parry")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Block, "State.Combat.Block")
	
	
	/** SS */ 
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_RightDownwardSlash, "State.Combat.Sword.RightDownwardSlash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_LeftUpwardSlash, "State.Combat.Sword.LeftUpwardSlash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_RightSpinJumpSlash, "State.Combat.Sword.RightSpinJumpSlash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_HiltStrike, "State.Combat.Sword.HiltStrike")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_Kick, "State.Combat.Sword.Kick")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_PowerUp, "State.Combat.Sword.PowerUp")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_RightSpinSlash1, "State.Combat.Sword.RightSpinSlash1")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_RightSpinSlash2, "State.Combat.Sword.RightSpinSlash2")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_TripleSlash, "State.Combat.Sword.TripleSlash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Sword_JumpSlash, "State.Combat.Sword.JumpSlash")
	/** end SS */
	
	/** GS */
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_GreatSword_RightDownwardSlash, "State.Combat.GreatSword.RightDownwardSlash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_GreatSword_LeftSpinSlash, "State.Combat.GreatSword.LeftSpinSlash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_GreatSword_LeftSpinJumpSlash, "State.Combat.GreatSword.LeftSpinJumpSlash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_GreatSword_RightSlideSlash, "State.Combat.GreatSword.RightSlideSlash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_GreatSword_RightUpwardStrike, "State.Combat.GreatSword.RightUpwardStrike")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_GreatSword_VerticalDownwardSlash, "State.Combat.GreatSword.VerticalDownwardSlash")
	/** end GS */
	
	/** Kraken */
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Kraken_ForwardSmash, "State.Combat.Kraken.ForwardSmash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Kraken_LeftSmash, "State.Combat.Kraken.LeftSmash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Kraken_RightSmash, "State.Combat.Kraken.RightSmash")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Kraken_TurnLeft, "State.Combat.Kraken.TurnLeft")
	UE_DEFINE_GAMEPLAY_TAG(State_Combat_Kraken_TurnRight, "State.Combat.Kraken.TurnRight")
	/** end Kraken */
	
	/** 
	 * Ability
	 */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Common_Run, "Ability.Active.Common.Run")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Common_Roll, "Ability.Active.Common.Roll")
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Passive_Common_GetHit, "Ability.Passive.Common.GetHit")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Passive_Common_BeCounter, "Ability.Passive.Common.BeCounter")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Passive_Common_Death, "Ability.Passive.Common.Death")
	
	/** Projectile */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Common_FireProjectile, "Ability.Active.Common.FireProjectile")
	/** end Projectile */
	
	/** SS */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_RightDownwardSlash, "Ability.Active.Sword.RightDownwardSlash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_LeftUpwardSlash, "Ability.Active.Sword.LeftUpwardSlash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_RightSpinJumpSlash, "Ability.Active.Sword.RightSpinJumpSlash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_HiltStrike, "Ability.Active.Sword.HiltStrike")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_Kick, "Ability.Active.Sword.Kick")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_PowerUp, "Ability.Active.Sword.PowerUp")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_RightSpinSlash1, "Ability.Active.Sword.RightSpinSlash1")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_RightSpinSlash2, "Ability.Active.Sword.RightSpinSlash2")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_TripleSlash, "Ability.Active.Sword.TripleSlash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Shword_JumpSlash, "Ability.Active.Sword.JumpSlash")
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Sword_Parry, "Ability.Active.Sword.Parry")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Sword_Block, "Ability.Active.Sword.Block")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Sword_RightUpwardSlash, "Ability.Active.Sword.RightUpwardSlash")
	/** end SS */
	
	/** GS */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_GreatSword_RightDownwardSlash, "Ability.Active.GreatSword.RightDownwardSlash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_GreatSword_LeftSpinSlash, "Ability.Active.GreatSword.LeftSpinSlash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_GreatSword_LeftSpinJumpSlash, "Ability.Active.GreatSword.LeftSpinJumpSlash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_GreatSword_RightSlideSlash, "Ability.Active.GreatSword.RightSlideSlash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_GreatSword_VerticalDownwardSlash, "Ability.Active.GreatSword.VerticalDownwardSlash")
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_GreatSword_Parry, "Ability.Active.GreatSword.Parry")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_GreatSword_Block, "Ability.Active.GreatSword.Block")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_GreatSword_RightUpwardStrike, "Ability.Active.GreatSword.RightUpwardStrike")
	/** end GS */

	/** Kraken */
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Kraken_ForwardSmash, "Ability.Active.Kraken.ForwardSmash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Kraken_LeftSmash, "Ability.Active.Kraken.LeftSmash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Kraken_RightSmash, "Ability.Active.Kraken.RightSmash")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Kraken_TurnLeft, "Ability.Active.Kraken.TurnLeft")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Active_Kraken_TurnRight, "Ability.Active.Kraken.TurnRight")
	/** end Kraken */
	
	
	/** 
	 * Set By Caller
	 */
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Ability_MotionValue, "SetByCaller.Ability.MotionValue")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Ability_ElementalMotionValue, "SetByCaller.Ability.ElementalMotionValue")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Weapon_Attack, "SetByCaller.Weapon.Attack")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Weapon_ElementalAttack, "SetByCaller.Weapon.ElementalAttack")
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Weapon_CriticalChance, "SetByCaller.Weapon.CriticalChance")

	
	/**
	 * Input
	 */
	UE_DEFINE_GAMEPLAY_TAG(Input_LightAttack, "Input.LightAttack")
	UE_DEFINE_GAMEPLAY_TAG(Input_HeavyAttack, "Input.HeavyAttack")
	UE_DEFINE_GAMEPLAY_TAG(Input_Run, "Input.Run")
	UE_DEFINE_GAMEPLAY_TAG(Input_Walk, "Input.Walk")
	UE_DEFINE_GAMEPLAY_TAG(Input_Roll, "Input.Roll")
	UE_DEFINE_GAMEPLAY_TAG(Input_Block, "Input.Block")

	
	/** 
	 * Item
	 */
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Sword, "Item.Weapon.Sword")
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_GreatSword, "Item.Weapon.GreatSword")

	
	/**
	 * Gameplay Cue
	 */
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Combat_DamageNumber, "GameplayCue.Combat.DamageNumber")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Combat_HitStop, "GameplayCue.Combat.HitStop")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Combat_HitNiagara, "GameplayCue.Combat.HitNiagara")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Combat_ImpactSound, "GameplayCue.Combat.ImpactSound")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Combat_ChargeNiagara, "GameplayCue.Combat.ChargeNiagara")
}
