#include "Inventory/VM_InventorySlot.h"

#include "Interaction/PlayerInterface.h"
#include "Inventory/Data/ItemDataAsset.h"

void UVM_InventorySlot::UpdateSlotData(UItemDataAsset* InItemAsset, const int32 InQuantity, const FGuid InItemGuid, const bool InBIsEquipped)
{
	SetItemAsset(InItemAsset);
	SetQuantity(InQuantity);
	SetItemGuid(InItemGuid);
	SetbIsEquipped(InBIsEquipped);
}

inline void UVM_InventorySlot::SlotClicked(UItemDataAsset* InItemAsset, const FGuid InItemGuid) const
{
	// If is Weapon, equip or unequip
	if (InItemAsset->ItemType == EItemType::Weapon)
	{
		if (bIsEquipped)
		{
			UnequipWeapon(InItemGuid);
		}
		else
		{
			EquipWeapon(InItemGuid);
		}
	}
	// If is Consumable, use consumable
	else if (InItemAsset->ItemType == EItemType::Consumable)
	{
		if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(PC->GetPawn()))
		{
			PlayerInterface->UseConsumable(InItemGuid);
		}
	}
}

void UVM_InventorySlot::EquipWeapon(const FGuid InItemGuid) const
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(PC->GetPawn()))
	{
		PlayerInterface->EquipWeapon(InItemGuid);
	}
}

void UVM_InventorySlot::UnequipWeapon(FGuid InItemGuid) const
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(PC->GetPawn()))
	{
		PlayerInterface->UnequipWeapon();
	}
}

void UVM_InventorySlot::SetItemAsset(UItemDataAsset* InItemDataAsset)
{
	UE_MVVM_SET_PROPERTY_VALUE(ItemAsset, InItemDataAsset);
}

void UVM_InventorySlot::SetQuantity(const int32 InQuantity)
{
	UE_MVVM_SET_PROPERTY_VALUE(Quantity, InQuantity);
}

void UVM_InventorySlot::SetItemGuid(const FGuid InItemGuid)
{
	ItemGuid = InItemGuid;
}

void UVM_InventorySlot::SetbIsEquipped(const bool InBIsEquipped)
{
	if (InBIsEquipped != bIsEquipped)
	{
		UE_MVVM_SET_PROPERTY_VALUE(bIsEquipped, InBIsEquipped);
	}
}
