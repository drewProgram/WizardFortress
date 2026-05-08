// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Systems/Inventory/InventoryTypes.h"

#include "AttributeSystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCharacterDied);
DECLARE_MULTICAST_DELEGATE(FOnManaChanged);
DECLARE_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_MULTICAST_DELEGATE(FOnMovementSpeedChanged);
DECLARE_MULTICAST_DELEGATE(FOnPDefenseChanged);
DECLARE_MULTICAST_DELEGATE(FOnEDefenseChanged);

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

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	int32 GetMana();

	UFUNCTION(BlueprintCallable)
	float GetMovementSpeed();

	UFUNCTION(BlueprintCallable)
	int32 GetPhysicalDefense();

	UFUNCTION(BlueprintCallable)
	int32 GetElementalDefense();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitAttributes();

	void UpdateMana(int32 TreatedValue);
	void UpdateHealth(float TreatedValue);

	UFUNCTION()
	void HandleEquipmentChanged(EEquipmentSlot Slot, UEquipmentData* NewItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Runtime Attributes")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Runtime Attributes")
	int32 Mana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Runtime Attributes")
	float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Attributes")
	int32 PhysicalDefense = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Attributes")
	int32 ElementalDefense = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Attributes")
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Attributes")
	int32 MaxMana = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Attributes")
	float MaxMovementSpeed = 500.f;

	ABaseCharacter* CompOwner;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
