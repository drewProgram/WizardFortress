#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include <GameplayTagContainer.h>

#include "Systems/Inventory/InventoryTypes.h"

#include "ItemData.generated.h"

UCLASS()
class WIZARDFORTRESS_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FText Description;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    EItemRarity Rarity = EItemRarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    bool bIsStackable = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (EditCondition = "bIsStackable", ClampMin = "1"))
    int32 MaxStackSize = 99;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    int32 SellPrice = 10;

    // Mesh quando dropado no mundo
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    UStaticMesh* WorldMesh = nullptr;

    // Para identificação única
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("Item", GetFName());
    }

    UFUNCTION(BlueprintPure)
    const EItemCategory GetItemCategory() { return ItemCategory; }

protected:
    UPROPERTY(BlueprintGetter = GetItemCategory, VisibleAnywhere)
    EItemCategory ItemCategory = EItemCategory::Misc;
};
