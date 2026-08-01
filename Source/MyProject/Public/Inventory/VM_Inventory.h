#pragma once

#include "CoreMinimal.h"
#include "UI/VM_MyViewModelBase.h"
#include "VM_Inventory.generated.h"

struct FInventoryEntry;
class AMyCharacter;
struct FInventorySlot;
class UVM_InventorySlot;
/**
 * For Grid View in Inventory UI.
 */
UCLASS()
class MYPROJECT_API UVM_Inventory : public UVM_MyViewModelBase
{
	GENERATED_BODY()
	
public:
	void BindToDependencies(const AMyCharacter* TargetCharacter);
	
	UFUNCTION()
	void HandleSlotChanged(const FInventoryEntry& NewEntry);
	
	UFUNCTION()
	void HandleSlotAdded(const FInventoryEntry& NewEntry);
	
	UFUNCTION()
	void HandleSlotRemoved(const FGuid& InItemGuid);
	
	void LoadAndPopulateSlot(UVM_InventorySlot* SlotVM, const FInventoryEntry& Entry);
	
	void SetItemList(const TArray<UVM_InventorySlot*>& InItemList);

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Setter, meta=(AllowPrivateAccess=true))
	TArray<UVM_InventorySlot*> ItemList;
	
};
