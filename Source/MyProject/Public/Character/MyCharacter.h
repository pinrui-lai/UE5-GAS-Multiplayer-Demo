#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystem/Data/TagToComboTable.h"
#include "Actor/WeaponActor.h"
#include "Character/MyCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "MyCharacter.generated.h"

class AWeaponActor;
class UComboDataAsset;
class UStateTreeComponent;
class UItemDataAsset;
class UInventoryComponent;
struct FInventorySlot;
/**
 * Player Character.
 */
UCLASS()
class MYPROJECT_API AMyCharacter : public AMyCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AMyCharacter();
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Destroyed() override;

	/** Player Interface*/
	virtual void AddToLevel(int32 InLevel) override;
	virtual void AddToXP(int32 InXP) override;
	virtual void AddToAttributePoints(int32 InAttributePoints) override;
	
	virtual int32 GetPlayerLevel() override;
	virtual int32 GetXP() override;
	virtual int32 GetAttributePoints() override;
	
	virtual int32 FindLevelByXP(int32 XP) override;
	virtual int32 GetAttributePointsRewardFromInfo(int32 Level) override;
	
	virtual void EquipWeapon(FGuid InItemGuid) override;
	virtual void UnequipWeapon() override;
	virtual void UseConsumable(FGuid InItemGuid) override;
	virtual void AddItemToInventory(FPrimaryAssetId ItemAssetId, int32 Amount) override;
	/** end Player Interface*/
	
	/** Combat Interface*/
	virtual void Die() override;
	virtual FGameplayTag GetWeaponAbilityTag() const override { return EquippedWeaponAbilityTag; }
	virtual UAnimMontage* GetDeathAnimMontage() const override { return DeathAnimMontage; }
	virtual USkeletalMeshComponent* GetWeaponMeshComponent() const override { return SpawnedWeaponActor->WeaponMeshComponent; }
	virtual TArray<FName> GetWeaponSocketNames() const override { return EquippedWeaponSocketNames; }
	/** end Combat Interface*/
	
	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon(FGuid InItemGuid);
	
	void AttachWeaponAndSetData(int32 EquippedWeaponIndex, const UWeaponDataAsset* LoadedWeaponAsset);
	
	void SpawnAndAttachWeapon(const UWeaponDataAsset* LoadedWeaponAsset);
	
	// Update server's ASC ComboDataAsset and WeaponSockets. Give Weapon Ability. Apply Weapon State Effect. Clear all if LoadedWeaponAsset is nullptr
	void UpdateCombatDataFromWeapon(const UWeaponDataAsset* LoadedWeaponAsset);
	
	UFUNCTION(Server, Reliable)
	void ServerUnequipWeapon();
	
	UFUNCTION(Server, Reliable)
	void ServerUseConsumable(FGuid InItemGuid);
	
	void LoadFromSaveData();
	
	UFUNCTION()
	void OnRep_EquippedWeaponGuid();

protected:
	void AddStartupItems();
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup|Item")
	TArray<FPrimaryAssetId> StartupItemAssetIds;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag EquippedWeaponTypeTag;

private:
	void InitAbilityActorInfo();
	
	void OnWeaponClassLoaded(TSoftClassPtr<AWeaponActor> LoadedSoftClassPtr);
	
	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeaponGuid)
	FGuid EquippedWeaponGuid;
	
	UPROPERTY()
	TObjectPtr<AWeaponActor> SpawnedWeaponActor;
	
	TArray<FName> EquippedWeaponSocketNames;
	
	FActiveGameplayEffectHandle EquippedWeaponActivateEffectHandle;
	
	FGameplayAbilitySpecHandle EquippedWeaponAbilitySpecHandle;
	FGameplayTag EquippedWeaponAbilityTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess))
	TObjectPtr<UTagToComboTable> TagToComboTable;
	
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess))
	bool bRunning;
	
	float RunSpeed = 600.0f;
	float WalkSpeed = 300.0f;
};
