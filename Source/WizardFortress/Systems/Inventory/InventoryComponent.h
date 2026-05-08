#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemData.h"

#include "Systems/Inventory/InventoryTypes.h"

#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, class UItemData*, ItemData, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRemoved, FText, ItemName, int32, Quantity);

class UConsumableData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
    bool RemoveItem(FText ItemName, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool CanAddItem(UItemData* ItemData, int32 Quantity = 1);

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetItemCount(UItemData* ItemData) const;

    UPROPERTY(BlueprintReadOnly)
    UConsumableData* LastConsumableAdded;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FInventorySlot> Slots;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(UItemData* ItemData, bool bHasItem, int32 Amount = 1);

    void PrintItems();
};
