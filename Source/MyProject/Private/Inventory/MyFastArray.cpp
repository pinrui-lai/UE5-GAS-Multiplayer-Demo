# include "Inventory/MyFastArray.h"
#include "Inventory/InventoryComponent.h"

void FInventoryFastArray::PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize)
{
	const UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;
	
	for (const int32 Index : ChangedIndices)
	{
		InventoryComponent->OnItemChanged.Broadcast(Entries[Index]);
	}
}

void FInventoryFastArray::PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize)
{
	const UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;
	
	for (const int32 Index : AddedIndices)
	{
		InventoryComponent->OnItemAdded.Broadcast(Entries[Index]);
	}
}

void FInventoryFastArray::PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize)
{
	const UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent)) return;
	
	for (const int32 Index : RemovedIndices)
	{
		// Before actually deleting it from client Entries array, get its GUID and broadcast it to UI.
		InventoryComponent->OnItemRemoved.Broadcast(Entries[Index].ItemGuid);
	}
}
