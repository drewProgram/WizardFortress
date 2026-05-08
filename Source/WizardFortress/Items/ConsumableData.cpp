#include "Items/ConsumableData.h"

UConsumableData::UConsumableData()
{
    bIsStackable = true;
    MaxStackSize = 99;

    ItemCategory = EItemCategory::Consumable;
}

FPrimaryAssetId UConsumableData::GetPrimaryAssetId() const
{
    return FPrimaryAssetId("Consumable", GetFName());
}