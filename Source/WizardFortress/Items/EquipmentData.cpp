#include "Items/EquipmentData.h"

UEquipmentData::UEquipmentData()
{
    bIsStackable = false;

    ItemCategory = EItemCategory::Armor;
}

FPrimaryAssetId UEquipmentData::GetPrimaryAssetId() const
{
    return FPrimaryAssetId("Equipment", GetFName());
}