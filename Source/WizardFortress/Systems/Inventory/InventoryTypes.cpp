#include "Systems/Inventory/InventoryTypes.h"
#include "Items/ItemData.h"

bool FInventorySlot::CanStackWith(const UItemData* OtherItem) const
{
    return ItemData == OtherItem && ItemData && ItemData->bIsStackable;
}

bool FInventorySlot::IsEmpty() const
{
    return ItemData == nullptr || Quantity <= 0;
}