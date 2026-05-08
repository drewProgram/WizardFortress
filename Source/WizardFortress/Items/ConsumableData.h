// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemData.h"



#include "ConsumableData.generated.h"

UCLASS()
class WIZARDFORTRESS_API UConsumableData : public UItemData
{
	GENERATED_BODY()
	
public:
    UConsumableData();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    float HealthRestore = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    float ManaRestore = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    float StaminaRestore = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    float BuffDuration = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    USoundBase* ConsumeSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Consumable")
    UParticleSystem* ConsumeEffect;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
