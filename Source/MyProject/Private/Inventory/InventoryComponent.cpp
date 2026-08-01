#include "Inventory/InventoryComponent.h"

#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UInventoryComponent, InventorySlots);
}

void UInventoryComponent::GetPrimaryAssetAsync(const FPrimaryAssetId& AssetId, const FOnAssetLoadedDelegate& OnComplete)
{
	if (!AssetId.IsValid())
	{
		OnComplete.ExecuteIfBound(nullptr);
		return;
	}
	
	UAssetManager& AssetManager = UAssetManager::Get();
	
	// Asset already in Memory, broadcast OnComplete immediately
	if (UPrimaryDataAsset* LoadedAsset = Cast<UPrimaryDataAsset>(AssetManager.GetPrimaryAssetObject(AssetId)))
	{
		OnComplete.ExecuteIfBound(LoadedAsset);
		return;
	}
	
	// Asset NOT in Memory, async load
	TArray<FPrimaryAssetId> ToLoad;
	ToLoad.Add(AssetId);
	
	TWeakObjectPtr<UInventoryComponent> WeakThis = this;
	AssetManager.LoadPrimaryAssets(ToLoad, TArray<FName>(), FStreamableDelegate::CreateLambda([WeakThis, AssetId, OnComplete]()
	{
		const UInventoryComponent* StrongThis = WeakThis.Get();
		if (!StrongThis) return; // Inventory Component no longer exists

		const UAssetManager& AM = UAssetManager::Get();
		UPrimaryDataAsset* AsyncLoadedAsset = Cast<UPrimaryDataAsset>(AM.GetPrimaryAssetObject(AssetId));
		
		// async loading completed, broadcast OnComplete
		OnComplete.ExecuteIfBound(AsyncLoadedAsset);
	}));
}

void UInventoryComponent::AddItemByGuid(const FGuid InItemGuid, const int32 InAmount)
{
	for (int32 i = 0; i < InventorySlots.Entries.Num(); i++)
	{
		if (InventorySlots.Entries[i].ItemGuid == InItemGuid)
		{
			InventorySlots.Entries[i].Quantity += InAmount;
			
			if (InventorySlots.Entries[i].Quantity <= 0)
			{
				RemoveItem(InventorySlots.Entries[i].ItemGuid);
				return;
			}
			
			InventorySlots.MarkItemDirty(InventorySlots.Entries[i]);
			OnItemChanged.Broadcast(InventorySlots.Entries[i]);
			break;
		}
	}
}

void UInventoryComponent::AddItemByPrimaryAssetId(const FPrimaryAssetId InPrimaryAssetId, const int32 InAmount)
{
	if (!GetOwner()->HasAuthority() || !InPrimaryAssetId.IsValid()) return;
	
	for (int32 i = InventorySlots.Entries.Num() - 1; i >= 0; i--)
	{
		// This kind of item already exist in inventory
		if (InventorySlots.Entries[i].ItemAssetId == InPrimaryAssetId)
		{
			InventorySlots.Entries[i].Quantity += InAmount;
			
			// If Quantity <= 0, remove the Item and return
			if (InventorySlots.Entries[i].Quantity <= 0)
			{
				RemoveItem(InventorySlots.Entries[i].ItemGuid);
				return;
			}
			
			InventorySlots.MarkItemDirty(InventorySlots.Entries[i]);
			OnItemChanged.Broadcast(InventorySlots.Entries[i]);
			return;
		}
	}
	
	// This kind of item is NOT yet exist in inventory, add a new slot.
	if (InAmount <= 0) return;
	const FInventoryEntry& NewEntry = InventorySlots.Entries.Add_GetRef(FInventoryEntry(InPrimaryAssetId, InAmount, FGuid::NewGuid()));
	InventorySlots.MarkItemDirty(InventorySlots.Entries[InventorySlots.Entries.Num() - 1]);
	OnItemAdded.Broadcast(NewEntry);
}

void UInventoryComponent::RemoveItem(const FGuid InGuid)
{
	for (int32 i = 0; i < InventorySlots.Entries.Num(); i++)
	{
		if (InventorySlots.Entries[i].ItemGuid == InGuid)
		{
			InventorySlots.Entries.RemoveAt(i);
			InventorySlots.MarkArrayDirty();
			OnItemRemoved.Broadcast(InGuid);
			break;
		}
	}
}

void UInventoryComponent::RemoveAll()
{
	for (int32 i = InventorySlots.Entries.Num() - 1; i >= 0 ; i--)
	{
		FGuid RemovedGuid = InventorySlots.Entries[i].ItemGuid;
		InventorySlots.Entries.RemoveAt(i);
		OnItemRemoved.Broadcast(RemovedGuid);
	}
	InventorySlots.MarkArrayDirty();
}

void UInventoryComponent::GetAssetIdAndIndexByGuid(const FGuid InItemGuid, int32& OutItemIndex,
                                                   FPrimaryAssetId& OutItemAssetId) const
{
	for (int32 i = 0; i < InventorySlots.Entries.Num(); i++)
	{
		if (InventorySlots.Entries[i].ItemGuid == InItemGuid)
		{
			OutItemAssetId = InventorySlots.Entries[i].ItemAssetId;
			OutItemIndex = i;
			break;
		}
	}
}

void UInventoryComponent::SetEntryEquipped(const int32 InIndex, const bool InBIsEquipped)
{
	if (InventorySlots.Entries[InIndex].bIsEquipped != InBIsEquipped)
	{
		InventorySlots.Entries[InIndex].bIsEquipped = InBIsEquipped;
		InventorySlots.MarkItemDirty(InventorySlots.Entries[InIndex]);
		OnItemChanged.Broadcast(InventorySlots.Entries[InIndex]);
	}
}


