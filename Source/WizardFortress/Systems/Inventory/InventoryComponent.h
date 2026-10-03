#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemData.h"

#include "Systems/Inventory/InventoryTypes.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnInventoryChanged,
	const FInventoryChange&,
	Change);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemAdded, class UItemData*, ItemData, int32, Quantity, FGuid, ItemId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRemoved, FGuid, ItemId, int32, Quantity);

class UConsumableData;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WIZARDFORTRESS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemRemoved OnItemRemoved;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(const FGuid& InstanceId, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryAddItem(UItemData* ItemData, int32 Quantity = 1);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetItemCount(UItemData* ItemData) const;

	UPROPERTY(BlueprintReadOnly)
	UConsumableData* LastConsumableAdded;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<FInventorySlot> GetSlots() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 TransferItemTo(UInventoryComponent* OtherInventory, FGuid ItemSlotId, int32 Amount);

protected:
	UPROPERTY(EditAnywhere, BlueprintGetter = GetSlots, Category = "Inventory")
	TArray<FInventorySlot> Slots;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemData* ItemData, bool bHasItem, int32 Amount = 1);

	void PrintItems();
};
