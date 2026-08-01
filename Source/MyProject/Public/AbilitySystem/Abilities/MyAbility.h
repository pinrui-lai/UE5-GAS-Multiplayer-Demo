#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MyAbility.generated.h"

class UMyAbilitySystemComponent;
/**
 * UMyAbility
 * * A base Gameplay Ability class custom-tailored for `Combo System` and `Action State Tracking`.
 * * Core Features & Flow:
 * 1. [Instancing Policy]: Defaults to `InstancedPerActor`.
 * 2. [Combo Window Management]: If `bIsCombatAbility` is true, remove Window_Combo tag right after ability activated to limit Combo Window.
 * 3. [Dynamic State Registration]: `ActivateAbility` filters the ability's `ActivationOwnedTags` 
 *    for tags containing "State" root, then assigns it to `UMyAbilitySystemComponent`'s `CurrentStateTag`. 
 *    This allows `ComboDataAsset` to find next ability in combo.
 * 4. [Lifecycle & AI Transition Notifications]: `EndAbility` clears the ASC's `CurrentStateTag`.
 *    and sends an `Event_Ability_Lifecycle_End` gameplay event to the avatar actor.
 */
UCLASS()
class MYPROJECT_API UMyAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMyAbility();
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY()
	TObjectPtr<UMyAbilitySystemComponent> MyAbilitySystemComponent;
	
	/**
	* If is Combat Ability first remove Window_Combo to limit Combo Window.
	* Combo Window in Combat Ability will be managed by AnimNotifyState from montage.
	*/
	UPROPERTY(EditDefaultsOnly, Category="Setup")
	bool bIsCombatAbility = true;
};
