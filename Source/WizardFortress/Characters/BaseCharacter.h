// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Systems/Inventory/InventoryTypes.h"

#include "BaseCharacter.generated.h"

class UInventoryComponent;
class UEquipmentComponent;
class USkillManager;
class UAttributeSystem;
class UEquipmentData;
class UConsumableManager;

UCLASS()
class WIZARDFORTRESS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	FText DisplayName;

	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintPure)
	UEquipmentComponent* GetEquipmentComponent() const;

	UFUNCTION(BlueprintPure)
	USkillManager* GetSkillComponent() const;

	UFUNCTION(BlueprintPure)
	UAttributeSystem* GetAttributeComponent() const;

	UFUNCTION(BlueprintPure)
	UConsumableManager* GetConsumableComponent() const;

	UFUNCTION(BlueprintPure)
	USceneComponent* GetSkillSpawnPoint() const;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetInventoryComponent)
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetEquipmentComponent)
	TObjectPtr<UEquipmentComponent> EquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetSkillComponent)
	TObjectPtr<USkillManager> SkillManager;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetAttributeComponent)
	TObjectPtr<UAttributeSystem> AttributeSystem;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetConsumableComponent)
	TObjectPtr<UConsumableManager> ConsumableManager;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetSkillSpawnPoint)
	TObjectPtr<USceneComponent> SkillSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TMap<EEquipmentSlot, UEquipmentData*> DefaultEquipments;

	void ApplyDefaultEquipment();

	virtual void HandleDeath();

};
