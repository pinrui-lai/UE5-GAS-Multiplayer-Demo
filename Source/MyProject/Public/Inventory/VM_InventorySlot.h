#pragma once

#include "CoreMinimal.h"
#include "UI/VM_MyViewModelBase.h"
#include "VM_InventorySlot.generated.h"

struct FInventoryEntry;
class UWeaponDataAsset;
class UItemDataAsset;
/**
 * For Grid View's Entry in Inventory UI.
 */
UCLASS()
class MYPROJECT_API UVM_InventorySlot : public UVM_MyViewModelBase
{
	GENERATED_BODY()
	
public:
	void UpdateSlotData(UItemDataAsset* InItemAsset, int32 InQuantity, FGuid InItemGuid, bool InBIsEquipped);
	
	UFUNCTION(BlueprintCallable)
	void SlotClicked(UItemDataAsset* InItemAsset, FGuid ItemGuid) const;
	
	void EquipWeapon(FGuid InItemGuid) const;
	void UnequipWeapon(FGuid InItemGuid) const;
	
	void SetItemAsset(UItemDataAsset* InItemDataAsset);
	void SetQuantity(int32 InQuantity);
	void SetItemGuid(FGuid InItemGuid);
	void SetbIsEquipped(bool InBIsEquipped);
	
	UItemDataAsset* GetItemAsset() const { return ItemAsset; };
	int32 GetQuantity() const { return Quantity; }
	FGuid GetItemGuid() const { return ItemGuid; }
	bool GetbIsEquipped() const { return bIsEquipped; }
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	TObjectPtr<UItemDataAsset> ItemAsset;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	int32 Quantity;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	FGuid ItemGuid;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Setter, meta = (AllowPrivateAccess = true))
	bool bIsEquipped;
	
};


