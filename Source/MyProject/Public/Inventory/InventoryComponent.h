#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/MyFastArray.h"
#include "InventoryComponent.generated.h"

struct FInventoryFastArray;
class UItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChange, const FInventoryEntry&, NewEntry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemRemove, const FGuid&, RemovedGuid);
DECLARE_DELEGATE_OneParam(FOnAssetLoadedDelegate, UPrimaryDataAsset*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FInventoryItemChange OnItemChanged;
	FInventoryItemChange OnItemAdded;
	FInventoryItemRemove OnItemRemoved;
	
	UInventoryComponent();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void GetPrimaryAssetAsync(const FPrimaryAssetId& AssetId, const FOnAssetLoadedDelegate& OnComplete);
	
	// To reduce item.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemByGuid(FGuid InItemGuid, int32 InAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemByPrimaryAssetId(FPrimaryAssetId InPrimaryAssetId, int32 InAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FGuid InGuid);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveAll();
	
	void GetAssetIdAndIndexByGuid(FGuid InItemGuid, int32& OutItemIndex, FPrimaryAssetId& OutItemAssetId) const;
	
	TArray<FInventoryEntry>& GetAllEntries() { return InventorySlots.Entries; }
	
	void SetEntryEquipped(int32 InIndex, bool InBIsEquipped);

private:
	UPROPERTY(Replicated)
	FInventoryFastArray InventorySlots = FInventoryFastArray(this);
	
};
