// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/AttributeSystem.h"

#include "Characters/BaseCharacter.h"
#include "Items/EquipmentData.h"
#include "Items/WeaponData.h"
#include "Systems/Inventory/EquipmentComponent.h"

// Sets default values for this component's properties
UAttributeSystem::UAttributeSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Mana = MaxMana;
	Health = MaxHealth;
	MovementSpeed = MaxMovementSpeed;

	// ...
}

bool UAttributeSystem::RequestUpdateMana(int32 BaseValue)
{
	int32 NewValue = BaseValue;
	if (BaseValue == 0)
	{
		return true;
	}

	if (BaseValue < 0)
	{
		if (Mana + BaseValue >= 0)
		{
			UpdateMana(BaseValue);
			return true;
		}

		UE_LOG(LogTemp, Warning, TEXT("Not enough mana!"));

		return false;
	}

	if (Mana + BaseValue > MaxMana)
	{
		Mana = MaxMana;
		return true;
	}
	UpdateMana(NewValue);
	
	return true;
}

bool UAttributeSystem::RequestUpdateHealth(float BaseValue)
{
	if (Health + BaseValue > MaxHealth)
	{
		Health = MaxHealth;
		return true;
	}
	UpdateHealth(BaseValue);

	return true;
}

float UAttributeSystem::GetHealth() const
{
	return Health;
}

float UAttributeSystem::GetMaxHealth() const
{
	return MaxHealth;
}

int32 UAttributeSystem::GetMana() const
{
	return Mana;
}

int32 UAttributeSystem::GetMaxMana() const
{
	return MaxMana;
}

float UAttributeSystem::GetMovementSpeed() const
{
	return MovementSpeed;
}

float UAttributeSystem::GetMaxMovementSpeed() const
{
	return 0.0f;
}

int32 UAttributeSystem::GetPhysicalDefense() const
{
	return PhysicalDefense;
}

int32 UAttributeSystem::GetElementalDefense() const
{
	return ElementalDefense;
}


// Called when the game starts
void UAttributeSystem::BeginPlay()
{
	Super::BeginPlay();

	ABaseCharacter* BaseOwner = Cast<ABaseCharacter>(GetOwner());
	if (BaseOwner)
	{
		CompOwner = BaseOwner;
		BaseOwner->GetEquipmentComponent()->OnEquipmentChanged.AddDynamic(this, &UAttributeSystem::HandleEquipmentChanged);

		Mana = MaxMana;
		Health = MaxHealth;
		MovementSpeed = MaxMovementSpeed;
	}
}

void UAttributeSystem::InitAttributes()
{
}

void UAttributeSystem::UpdateMana(int32 TreatedValue)
{
	Mana += TreatedValue;
	OnManaChanged.Broadcast(Mana, MaxMana);
	UE_LOG(LogTemp, Display, TEXT("New mana value: %d"), Mana);
}

void UAttributeSystem::UpdateHealth(float TreatedValue)
{
	Health += TreatedValue;
	UE_LOG(LogTemp, Display, TEXT("New health value: %f"), Health);

	OnHealthChanged.Broadcast(Health, MaxHealth);

	if (Health <= 0.f)
	{
		OnCharacterDied.Broadcast();
	}
}

void UAttributeSystem::HandleEquipmentChanged(EEquipmentSlot Slot, UEquipmentData* NewItem)
{
	UEquipmentData* OldItem = CompOwner->GetEquipmentComponent()->GetEquippedItem(Slot);
	if (OldItem)
	{
		if (OldItem->GetItemCategory() == EItemCategory::Weapon)
		{
			UWeaponData* WeaponData = Cast<UWeaponData>(OldItem);
			UE_LOG(LogTemp, Display, TEXT("Era uma arma!!!"));
		}

		// remove old item bonus
		MaxHealth -= OldItem->HealthBonus;
		MaxMana -= OldItem->ManaBonus;
		PhysicalDefense -= OldItem->ArmorValue;

		if (NewItem)
		{
			// add new bonus if has new equip
			MaxHealth += NewItem->HealthBonus;
			MaxMana += NewItem->ManaBonus;
			PhysicalDefense += NewItem->ArmorValue;
		}
		UE_LOG(LogTemp, Display, TEXT("Updated attributes for new equipment!"));

		// call delegate for ui here
		OnHealthChanged.Broadcast(Health, MaxHealth);
		OnManaChanged.Broadcast(Mana, MaxMana);
		OnPDefenseChanged.Broadcast(PhysicalDefense);

		return;
	}

	// reach here if player had no equip in slot
	MaxHealth += NewItem->HealthBonus;
	MaxMana += NewItem->ManaBonus;
	PhysicalDefense += NewItem->ArmorValue;

	OnHealthChanged.Broadcast(Health, MaxHealth);
	OnManaChanged.Broadcast(Mana, MaxMana);
	OnPDefenseChanged.Broadcast(PhysicalDefense);

	UE_LOG(LogTemp, Display, TEXT("Updated attributes for new equipment!"));
}


// Called every frame
void UAttributeSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

