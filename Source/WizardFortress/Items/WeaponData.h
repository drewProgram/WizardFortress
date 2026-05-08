// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/EquipmentData.h"



#include "WeaponData.generated.h"

UCLASS()
class WIZARDFORTRESS_API UWeaponData : public UEquipmentData
{
	GENERATED_BODY()

public:
    UWeaponData();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats")
    int32 MinDamage = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats")
    int32 MaxDamage = 15;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats")
    float AttackSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Stats")
    float AttackRange = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Animation")
    TSubclassOf<UAnimInstance> AnimationOverride;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|VFX")
    UParticleSystem* HitEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Audio")
    USoundBase* SwingSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Audio")
    USoundBase* HitSound;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	
};
