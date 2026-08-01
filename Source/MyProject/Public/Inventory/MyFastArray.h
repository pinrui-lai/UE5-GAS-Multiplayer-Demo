#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "MyFastArray.generated.h"

struct FInventoryFastArray;
class UItemDataAsset;
class UInventoryComponent;

/** A Single entry in an inventory */
USTRUCT()
struct FInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
public:
	FInventoryEntry() {}
	FInventoryEntry(const FPrimaryAssetId& InItemAssetId, const int32 InQuantity, const FGuid InItemGuid) : ItemAssetId(InItemAssetId), Quantity(InQuantity), ItemGuid(InItemGuid) {}
	
	UPROPERTY()
	FPrimaryAssetId ItemAssetId;
	
	UPROPERTY()
	int32 Quantity = 0;
	
	UPROPERTY()
	FGuid ItemGuid;
	
	UPROPERTY()
	bool bIsEquipped = false;
};


/** List of inventory items */
USTRUCT()
struct FInventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()
	
	FInventoryFastArray() : OwnerComponent(nullptr) {}
	explicit FInventoryFastArray(UActorComponent* InOwnerComponent) :  OwnerComponent(InOwnerComponent) {}
	
	/** FFastArraySerializer contract */
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	/** end FFastArraySerializer contract */
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FInventoryEntry, FInventoryFastArray>(Entries, DeltaParams, *this);
	}
	
private:
	friend class UInventoryComponent;
	
	UPROPERTY()
	TArray<FInventoryEntry> Entries;
	
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FInventoryFastArray> : public TStructOpsTypeTraitsBase2<FInventoryFastArray>
{
	enum  { WithNetDeltaSerializer = true };
};