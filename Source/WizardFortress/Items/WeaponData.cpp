#include "Items/WeaponData.h"

UWeaponData::UWeaponData()
{
    EquipmentSlot = EEquipmentSlot::MainHand;
    WeaponType = EWeaponType::Sword;
    ItemCategory = EItemCategory::Weapon;
}

FPrimaryAssetId UWeaponData::GetPrimaryAssetId() const
{
    return FPrimaryAssetId("Weapon", GetFName());
}