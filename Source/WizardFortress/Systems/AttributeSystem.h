// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Systems/Inventory/InventoryTypes.h"

#include "AttributeSystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCharacterDied);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, int32, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementSpeedChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPDefenseChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEDefenseChanged, int32);

class UEquipmentData;
class ABaseCharacter;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WIZARDFORTRESS_API UAttributeSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeSystem();

	UFUNCTION(BlueprintCallable)
	bool RequestUpdateMana(int32 BaseValue);

	UFUNCTION(BlueprintCallable)
	bool RequestUpdateHealth(float BaseValue);

	FOnCharacterDied OnCharacterDied;

	/* Attribute Update Delegates */
	FOnManaChanged           OnManaChanged;
	FOnHealthChanged         OnHealthChanged;
	FOnMovementSpeedChanged  OnMovementSpeedChanged;
	FOnPDefenseChanged       OnPDefenseChanged;
	FOnEDefenseChanged       OnEDefenseChanged;

	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure)
	int32 GetMana() const;

	UFUNCTION(BlueprintPure)
	int32 GetMaxMana() const;

	UFUNCTION(BlueprintPure)
	float GetMovementSpeed() const;

	UFUNCTION(BlueprintPure)
	float GetMaxMovementSpeed() const;

	UFUNCTION(BlueprintPure)
	int32 GetPhysicalDefense() const;

	UFUNCTION(BlueprintPure)
	int32 GetElementalDefense() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitAttributes();

	void UpdateMana(int32 TreatedValue);
	void UpdateHealth(float TreatedValue);

	UFUNCTION()
	void HandleEquipmentChanged(EEquipmentSlot Slot, UEquipmentData* NewItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter = GetHealth, Category = "Runtime Attributes")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter = GetMana, Category = "Runtime Attributes")
	int32 Mana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter = GetMovementSpeed, Category = "Runtime Attributes")
	float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetPhysicalDefense, Category = "Base Attributes")
	int32 PhysicalDefense = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetElementalDefense, Category = "Base Attributes")
	int32 ElementalDefense = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetMaxHealth, Category = "Base Attributes")
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetMaxMana, Category = "Base Attributes")
	int32 MaxMana = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, BlueprintGetter = GetMaxMovementSpeed, Category = "Base Attributes")
	float MaxMovementSpeed = 500.f;

	ABaseCharacter* CompOwner;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
