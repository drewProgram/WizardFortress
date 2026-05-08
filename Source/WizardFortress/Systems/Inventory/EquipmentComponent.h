#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class UEquipmentData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentSlot, Slot, UEquipmentData*, NewItem);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WIZARDFORTRESS_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UEquipmentComponent();

    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FOnEquipmentChanged OnEquipmentChanged;

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool EquipItem(UEquipmentData* EquipmentData);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool UnequipItem(EEquipmentSlot Slot);

    UFUNCTION(BlueprintCallable)
    void PrintEquippedItems();

    UFUNCTION(BlueprintPure, Category = "Equipment")
    UEquipmentData* GetEquippedItem(EEquipmentSlot Slot) const;

    UFUNCTION(BlueprintPure, Category = "Equipment")
    bool IsSlotOccupied(EEquipmentSlot Slot) const;

    UFUNCTION(BlueprintPure, Category = "Equipment")
    int32 GetTotalArmor() const;

    UFUNCTION(BlueprintPure, Category = "Equipment")
    int32 GetTotalAttackPower() const;

    UFUNCTION(BlueprintCallable)
    bool AreEquipmentsEmpty();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    TMap<EEquipmentSlot, UEquipmentData*> EquippedItems;

    // Cache dos meshes instanciados
    UPROPERTY()
    TMap<EEquipmentSlot, USkeletalMeshComponent*> EquipmentMeshes;

    virtual void BeginPlay() override;

    void UpdateEquipmentMesh(EEquipmentSlot Slot, UEquipmentData* EquipmentData);
    void RemoveEquipmentMesh(EEquipmentSlot Slot);
    void AttachWeaponToSocket(UEquipmentData* WeaponData, FName SocketName);

    void InitEquipmentSlots();
};
