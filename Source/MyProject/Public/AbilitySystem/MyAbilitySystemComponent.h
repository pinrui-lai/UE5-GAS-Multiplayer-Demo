#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

class UComboDataAsset;
/**
 * UMyAbilitySystemComponent
 * * A customized Ability System Component (ASC) that integrates `Tag-Driven Input Routing`, 
 *   `Input Buffering`, `Data-Driven Combo System`, and server-authoritative `Attribute Upgrade`.
 * * Core Architecture & Logic:
 * 1. [Tag-Driven Input Routing]: `AbilityInputPressed/Held/Released` triggers abilities via FGameplayTag,
 *      Key bindings can be modified easily by only changing the Input Mapping Context.
 *      `MyEnhancedInputComponent` binds all InputActions in InputConfig to Pressed, Released and Held, and passing their corresponding Input Tags as parameters.
 * 2. [Input Buffering & Combo Windowing]:
 *    - Pre-Input Window (`Window_PreInput`): During this window, Inputs are cached in `PreInputTagBuffer` instead of being dropped, even if Montage isn't in Combo Window.
 *    - Combo Window (`Window_Combo`): By listening to tag changes, once the Montage enters Combo Window, 
 *      the ASC automatically fetches the next Ability using the `PreInputTagBuffer` and `CurrentStateTag`, 
 *      then activates it. This significantly improves the responsiveness and smoothness of the action gameplay.
 * 3. [Data-Driven Combo Processing]:
 *    - When a Combo-Related Input is processed, it utilizes the `ComboDataAsset` to resolve and fetch the next Ability 
 *      based on the `CurrentStateTag` and the incoming Input Tag.
 * 4. [Authoritative Attribute Upgrading]: Attributes are securely modified only on the server. The Server RPC sends 
 *     a Gameplay Event, which is then received by a passive GA to handle the actual attribute modification.
 */
UCLASS()
class MYPROJECT_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	void UpgradeAttribute(FGameplayTag AttributeTag);
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses);
	void AddCharacterAbilitiesAndActivate(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses);
	
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	
	UFUNCTION()
	void SetCurrentStateTag(const FGameplayTag InCurrentStateTag) { CurrentStateTag = InCurrentStateTag; };
	
	void SetComboDataAsset(UComboDataAsset* InComboDataAsset) { ComboDataAsset = InComboDataAsset; };

private:
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(FGameplayTag AttributeTag);

	FGameplayAbilitySpec* FindAbilitySpecPtrByAssetTag(FGameplayTag AssetTag);
	
	// Get next ability's tag by searching ComboTable
	FGameplayAbilitySpec* ProcessComboInput(FGameplayTag InputTag);
	
	UFUNCTION()
	void OnWindowPreInputTagChanged(FGameplayTag InputTag, int32 NewCount);
	
	UFUNCTION()
	void OnWindowComboTagChanged(FGameplayTag InputTag, int32 NewCount);
	
	UFUNCTION()
	void OnStateCombatTagChanged(FGameplayTag InputTag, int32 NewCount);
	
	// For search next ability in combo
	UPROPERTY()
	TObjectPtr<UComboDataAsset> ComboDataAsset;
	
	// For search next ability in combo, will be assigned by MyAbility.
	FGameplayTag CurrentStateTag;
	
	FGameplayTag PreInputTagBuffer;
};
