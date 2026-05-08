#pragma once

#include "CoreMinimal.h"
#include "Items/ItemData.h"

#include "EquipmentData.generated.h"

UCLASS()
class WIZARDFORTRESS_API UEquipmentData : public UItemData
{
	GENERATED_BODY()
	
public:
    UEquipmentData();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    EEquipmentSlot EquipmentSlot = EEquipmentSlot::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
    USkeletalMesh* EquipmentMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Stats")
    int32 ArmorValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Stats")
    int32 HealthBonus = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Stats")
    int32 ManaBonus = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Requirements")
    int32 RequiredLevel = 1;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
