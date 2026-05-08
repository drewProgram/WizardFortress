#include "Systems/Inventory/InventoryComponent.h"

#include "Items/ConsumableData.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LastConsumableAdded = nullptr;
}

bool UInventoryComponent::AddItem(UItemData* ItemData, bool bHasItem, int32 Amount)
{
	if (bHasItem)
	{
		for (FInventorySlot& Slot : Slots)
		{
			if (Slot.ItemData->ItemName.EqualTo(ItemData->ItemName))
			{
				Slot.Quantity += Amount;

				PrintItems();

				return true;
			}
		}

		return false;
	}

	FInventorySlot NewSlot;
	NewSlot.ItemData = ItemData;
	NewSlot.Quantity = Amount;

	if (ItemData->GetItemCategory() == EItemCategory::Consumable)
	{
		LastConsumableAdded = Cast<UConsumableData>(ItemData);
	}

	Slots.Add(NewSlot);

	PrintItems();

	return true;
}

void UInventoryComponent::PrintItems()
{
	UE_LOG(LogTemp, Display, TEXT("-------------------------------------------------------"));
	UE_LOG(LogTemp, Display, TEXT("                     INVENTORY                         "));
	for (const FInventorySlot& Slot : Slots)
	{
		UE_LOG(LogTemp, Display, TEXT("Item Name: %s; Quantity: %d; Index: %d"), *Slot.ItemData->ItemName.ToString(), Slot.Quantity, Slot.SlotIndex);
	}
	UE_LOG(LogTemp, Display, TEXT("-------------------------------------------------------"));
}

bool UInventoryComponent::RemoveItem(FText ItemName, int32 Quantity)
{
	int32 Index = 0;
	bool bShouldRemoveFromArray = false;
	for (FInventorySlot& Slot : Slots)
	{
		if (Slot.ItemData->ItemName.EqualTo(ItemName))
		{
			if (Slot.Quantity - Quantity > 0)
			{
				Slot.Quantity -= Quantity;
				PrintItems();
				return true;
			}

			bShouldRemoveFromArray = true;
			break;
		}
		Index++;
	}

	if (bShouldRemoveFromArray)
	{
		Slots.RemoveAt(Index);
		PrintItems();
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Item is not on inventory, cannot remove it."));

	return false;
}

bool UInventoryComponent::CanAddItem(UItemData* ItemData, int32 Quantity)
{
	for (const FInventorySlot& Slot : Slots)
	{
		// checar se item existe no inventario
		if (Slot.ItemData->ItemName.EqualTo(ItemData->ItemName))
		{
			// checa se item stacka
			if (ItemData->bIsStackable)
			{
				// checar se item não passou do stack maximo
				if (Slot.Quantity + Quantity <= ItemData->MaxStackSize)
				{
					AddItem(ItemData, true, Quantity);
					return true;
				}
				return false;
			}
		}
	}

	AddItem(ItemData, false, Quantity);

	return true;
}

int32 UInventoryComponent::GetItemCount(UItemData* ItemData) const
{
	int32 Count = 0;

	for (const FInventorySlot& Slot : Slots)
	{
		if (ItemData->ItemName.EqualTo(Slot.ItemData->ItemName))
		{
			Count = Slot.Quantity;
			break;
		}
	}

	return Count;
}
