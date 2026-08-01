#include "Inventory/VM_Inventory.h"

#include "Character/MyCharacter.h"
#include "Engine/AssetManager.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Data/ItemDataAsset.h"
#include "Inventory/VM_InventorySlot.h"

void UVM_Inventory::BindToDependencies(const AMyCharacter* TargetCharacter)
{
	UInventoryComponent* InventoryComponent = TargetCharacter->FindComponentByClass<UInventoryComponent>();
	check(InventoryComponent)
	
	InventoryComponent->OnItemChanged.AddDynamic(this, &UVM_Inventory::HandleSlotChanged);
	InventoryComponent->OnItemAdded.AddDynamic(this, &UVM_Inventory::HandleSlotAdded);
	InventoryComponent->OnItemRemoved.AddDynamic(this, &UVM_Inventory::HandleSlotRemoved);
	
	// Broadcast for first time.
	ItemList.Empty();
	TArray<FInventoryEntry>& AllEntries = InventoryComponent->GetAllEntries();
	for (int32 i = 0; i < AllEntries.Num(); i++)
	{
		UVM_InventorySlot* NewSlotVM = NewObject<UVM_InventorySlot>(this);
		NewSlotVM->SetViewModelParams(FViewModelParams(PC.Get(), PS.Get(), ASC.Get(), AS.Get()));
		ItemList.Insert(NewSlotVM, i);
	
		LoadAndPopulateSlot(ItemList[i], AllEntries[i]);
	}
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ItemList);
}

void UVM_Inventory::HandleSlotChanged(const FInventoryEntry& NewEntry)
{
	for (UVM_InventorySlot* SlotVM : ItemList)
	{
		if (SlotVM && SlotVM->GetItemGuid() == NewEntry.ItemGuid)
		{
			LoadAndPopulateSlot(SlotVM, NewEntry);
			return;
		}
	}
}

void UVM_Inventory::HandleSlotAdded(const FInventoryEntry& NewEntry)
{
	// Avoid adding slots with the same GUID.
	for (UVM_InventorySlot* SlotVM : ItemList)
	{
		if (SlotVM && SlotVM->GetItemGuid() == NewEntry.ItemGuid)
		{
			LoadAndPopulateSlot(SlotVM, NewEntry);
			return;
		}
	}

	UVM_InventorySlot* NewSlotVM = NewObject<UVM_InventorySlot>(this);
	NewSlotVM->SetViewModelParams(FViewModelParams(PC.Get(), PS.Get(), ASC.Get(), AS.Get()));
	
	ItemList.Add(NewSlotVM);
	LoadAndPopulateSlot(NewSlotVM, NewEntry);
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ItemList);
}

void UVM_Inventory::HandleSlotRemoved(const FGuid& InItemGuid)
{
	// Find the UI slot to remove using GUID
	for (int32 i = 0; i < ItemList.Num(); i++)
	{
		if (ItemList[i] && ItemList[i]->GetItemGuid() == InItemGuid)
		{
			ItemList.RemoveAt(i);
			UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(ItemList);
			return;
		}
	}
}

void UVM_Inventory::LoadAndPopulateSlot(UVM_InventorySlot* SlotVM, const FInventoryEntry& Entry)
{
	if (!SlotVM) return;
	
	// If Quantity == 0, clear slot view model
	if (Entry.Quantity <=0 || !Entry.ItemAssetId.IsValid())
	{
		SlotVM->UpdateSlotData(nullptr, 0, FGuid(), false);
		return;
	}
	
	UAssetManager& AssetManager = UAssetManager::Get();

	// Update if asset already exist in memory
	if (UItemDataAsset* LoadedItem = Cast<UItemDataAsset>(AssetManager.GetPrimaryAssetObject(Entry.ItemAssetId)))
	{
		SlotVM->UpdateSlotData(LoadedItem, Entry.Quantity, Entry.ItemGuid, Entry.bIsEquipped);
	}
	// If asset is NOT yet in memory, load async
	else
	{
		TArray<FPrimaryAssetId> ToLoad;
		ToLoad.Add(Entry.ItemAssetId);

		// Use WeakPtr to prevent crashes caused by the UI or ViewModel being destroyed while the Lambda is executing.
		TWeakObjectPtr<UVM_InventorySlot> WeakSlotVM = SlotVM;

		AssetManager.LoadPrimaryAssets(ToLoad, TArray<FName>(), FStreamableDelegate::CreateLambda([WeakSlotVM, Entry]()
		{
			if (WeakSlotVM.IsValid())
			{
				UItemDataAsset* AsyncLoadedItem = Cast<UItemDataAsset>(UAssetManager::Get().GetPrimaryAssetObject(Entry.ItemAssetId));
				WeakSlotVM->UpdateSlotData(AsyncLoadedItem, Entry.Quantity, Entry.ItemGuid, Entry.bIsEquipped);
			}
		}));
	}
}

void UVM_Inventory::SetItemList(const TArray<UVM_InventorySlot*>& InItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(ItemList, InItemList);
}