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

UENUM(BlueprintType)
enum class EInventoryChangeType : uint8
{
    Added,
    QuantityChanged,
    Removed
};

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UItemData* ItemData = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;

    UPROPERTY(BlueprintReadOnly)
    FGuid InstanceId{};

    bool IsEmpty() const;
    
    bool CanStackWith(const UItemData* OtherItem) const;
};

USTRUCT(BlueprintType)
struct FInventoryChange
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EInventoryChangeType ChangeType;

    UPROPERTY(BlueprintReadOnly)
    FGuid InstanceId;

    UPROPERTY(BlueprintReadOnly)
    FInventorySlot Slot;
};