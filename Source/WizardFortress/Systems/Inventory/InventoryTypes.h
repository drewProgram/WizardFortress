#pragma once

#include "CoreMinimal.h"

#include "Misc/EnumRange.h"

#include "InventoryTypes.generated.h"

class UItemData;

/* 
   For well defined types (small categories that are not likely to increase later), it's better to use
   enumarators, not gameplay tags.
*/

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
    Armor,
    Weapon,
    Consumable,
    Key,
    Misc
};

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    None,
    Chest,
    Feet,
    MainHand,
    Ring,
    Necklace,
    MAX
};

ENUM_RANGE_BY_FIRST_AND_LAST(EEquipmentSlot, EEquipmentSlot::Chest, EEquipmentSlot::Necklace);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Sword,
    Axe,
    Bow,
    Staff,
    Dagger,
    Wand
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common,
    Rare,
    Epic,
    Legendary
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemData* ItemData = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SlotIndex = -1;

    bool IsEmpty() const;
    
    bool CanStackWith(const UItemData* OtherItem) const;
};