#include "Systems/Inventory/InventoryComponent.h"

#include "Items/ConsumableData.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LastConsumableAdded = nullptr;
}

bool UInventoryComponent::AddItem(UItemData* ItemData, bool bHasItem, int32 Amount)
{
	if (ItemData->bIsStackable)
	{
		FInventorySlot* ExistingSlot =
			Slots.FindByPredicate(
				[ItemData](const FInventorySlot& Slot)
				{
					return Slot.ItemData == ItemData;
				});

		if (ExistingSlot)
		{
			ExistingSlot->Quantity += Amount;
			
			FInventoryChange Change;
			Change.ChangeType = EInventoryChangeType::QuantityChanged;
			Change.Slot = *ExistingSlot;
			Change.InstanceId = ExistingSlot->InstanceId;

			OnInventoryChanged.Broadcast(Change);
			OnItemAdded.Broadcast(
				ExistingSlot->ItemData,
				ExistingSlot->Quantity,
				ExistingSlot->InstanceId
			);

			PrintItems();

			return true;
		}

		FInventorySlot NewSlot;
		FGuid InstanceId = FGuid::NewGuid();
		NewSlot.InstanceId = InstanceId;
		NewSlot.ItemData = ItemData;
		NewSlot.Quantity = Amount;

		Slots.Add(MoveTemp(NewSlot));

		FInventoryChange Change;
		Change.ChangeType = EInventoryChangeType::Added;
		Change.Slot = NewSlot;
		Change.InstanceId = InstanceId;

		OnInventoryChanged.Broadcast(Change);
		OnItemAdded.Broadcast(ItemData, Amount, InstanceId);

		PrintItems();

		return true;
	}
	else
	{
		// Cada unidade ocupa seu próprio slot.
		for (int32 Count = 0; Count < Amount; ++Count)
		{
			FInventorySlot NewSlot;
			FGuid InstanceId = FGuid::NewGuid();
			NewSlot.InstanceId = InstanceId;
			NewSlot.ItemData = ItemData;
			NewSlot.Quantity = 1;

			// Usando move semantics mais pra ter como referencia futuramente, mas o objeto de item é
			// pequeno o suficiente pra n ser problema fazer uma copia
			Slots.Add(MoveTemp(NewSlot));

			FInventoryChange Change;
			Change.ChangeType = EInventoryChangeType::Added;
			Change.Slot = NewSlot;
			Change.InstanceId = InstanceId;

			OnInventoryChanged.Broadcast(Change);
			OnItemAdded.Broadcast(ItemData, Amount, InstanceId);
			PrintItems();
		}
		return true;
	}
}

void UInventoryComponent::PrintItems()
{
	UE_LOG(LogTemp, Display, TEXT("-------------------------------------------------------"));
	UE_LOG(LogTemp, Display, TEXT("                     INVENTORY                         "));
	for (const FInventorySlot& Slot : Slots)
	{
		UE_LOG(LogTemp, Display, TEXT("Item Name: %s; Quantity: %d; FGuid: %s"), *Slot.ItemData->ItemName.ToString(), Slot.Quantity, *Slot.InstanceId.ToString());
	}
	UE_LOG(LogTemp, Display, TEXT("-------------------------------------------------------"));
}

bool UInventoryComponent::RemoveItem(const FGuid& InstanceId, int32 Quantity)
{
	const int32 SlotIndex = Slots.IndexOfByPredicate(
		[&InstanceId](const FInventorySlot& Slot)
		{
			return Slot.InstanceId == InstanceId;
		});

	if (SlotIndex == INDEX_NONE)
	{
		return false;
	}

	FInventorySlot& Slot = Slots[SlotIndex];

	FInventoryChange Change;
	Change.InstanceId = InstanceId;

	if (Slot.ItemData->bIsStackable)
	{
		if (Quantity <= 0 || Quantity > Slot.Quantity)
		{
			return false;
		}

		Slot.Quantity -= Quantity;

		if (Slot.Quantity == 0)
		{
			Slots.RemoveAt(SlotIndex);
			Change.ChangeType = EInventoryChangeType::Removed;
			OnInventoryChanged.Broadcast(Change);

			return true;
		}

		Change.ChangeType = EInventoryChangeType::QuantityChanged;
		Change.Slot = Slots[SlotIndex];
		OnInventoryChanged.Broadcast(Change);
	}
	else
	{
		// Um slot não-stackável representa uma unidade.
		Slots.RemoveAt(SlotIndex);
		Change.ChangeType = EInventoryChangeType::Removed;
		OnInventoryChanged.Broadcast(Change);
	}

	return true;
}

bool UInventoryComponent::TryAddItem(UItemData* ItemData, int32 Quantity)
{
	if (!IsValid(ItemData) || Quantity <= 0)
	{
		return false;
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

TArray<FInventorySlot> UInventoryComponent::GetSlots() const
{
	return Slots;
}

int32 UInventoryComponent::TransferItemTo(UInventoryComponent* OtherInventory, FGuid ItemSlotId, int32 Amount)
{
	FInventorySlot* ItemSlot =
		Slots.FindByPredicate(
			[ItemSlotId](const FInventorySlot& Slot)
			{
				return Slot.InstanceId == ItemSlotId;
			});

	if (ItemSlot)
	{
		if (OtherInventory->TryAddItem(ItemSlot->ItemData, Amount))
		{
			int32 AmountLeft = ItemSlot->Quantity - Amount;
			RemoveItem(ItemSlotId, Amount);

			return AmountLeft;
		}
	}

	return -1;
}
