#include "Systems/Inventory/EquipmentComponent.h"

#include "Items/EquipmentData.h"
#include "Systems/Inventory/InventoryTypes.h"
#include "Misc/EnumRange.h"
#include "Characters/BaseCharacter.h"

UEquipmentComponent::UEquipmentComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;

	InitEquipmentSlots();
}

bool UEquipmentComponent::EquipItem(UEquipmentData* EquipmentData)
{
	EEquipmentSlot Slot = EquipmentData->EquipmentSlot;

	if (IsSlotOccupied(Slot))
	{
		UnequipItem(Slot);
	}

	OnEquipmentChanged.Broadcast(Slot, EquipmentData);
	EquippedItems[Slot] = EquipmentData;

	PrintEquippedItems();

	if (Slot == EEquipmentSlot::MainHand)
	{
		if (EquipmentMeshes.IsEmpty())
		{
			USkeletalMeshComponent* WeaponComp = NewObject<USkeletalMeshComponent>(GetOwner());
			WeaponComp->RegisterComponent();
			WeaponComp->SetSkeletalMesh(EquipmentData->EquipmentMesh);

			// ATENÇÃO: SEMPRE usar add se vc sabe que a chave não existe. Engine vai crashar se tentar atribuir valor a uma chave que não existe, diferente do comportamento do std::map
			EquipmentMeshes.Add(EEquipmentSlot::MainHand, WeaponComp);

			AttachWeaponToSocket(EquipmentData, TEXT("WeaponSocket"));

			return true;
		}
		UpdateEquipmentMesh(Slot, EquipmentData);
	}
	return true;
}

void UEquipmentComponent::PrintEquippedItems()
{
	UE_LOG(LogTemp, Display, TEXT("-------------------------------------------------------"));
	UE_LOG(LogTemp, Display, TEXT("                   EQUIPMENTS                          "));

	for (const TPair<EEquipmentSlot, UEquipmentData*>& Pair : EquippedItems)
	{
		if (Pair.Value == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("Item Slot: %s; No item equipped;"), *UEnum::GetValueAsString(Pair.Key));
			continue;
		}
		UE_LOG(LogTemp, Display, TEXT("Item Slot: %s; Item name: %s;"), *UEnum::GetValueAsString(Pair.Key), *Pair.Value->ItemName.ToString());
	}
	UE_LOG(LogTemp, Display, TEXT("-------------------------------------------------------"));
}

bool UEquipmentComponent::UnequipItem(EEquipmentSlot Slot)
{
	if (Slot == EEquipmentSlot::MainHand)
	{
		RemoveEquipmentMesh(Slot);
	}

	OnEquipmentChanged.Broadcast(Slot, nullptr);
	EquippedItems[Slot] = nullptr;

	return true;
}

UEquipmentData* UEquipmentComponent::GetEquippedItem(EEquipmentSlot Slot) const
{
	return EquippedItems[Slot];
}

bool UEquipmentComponent::IsSlotOccupied(EEquipmentSlot Slot) const
{
	if (EquippedItems[Slot] != nullptr) return true;
	return false;
}

int32 UEquipmentComponent::GetTotalArmor() const
{
	return int32();
}

int32 UEquipmentComponent::GetTotalAttackPower() const
{
	return int32();
}

bool UEquipmentComponent::AreEquipmentsEmpty()
{
	for (const auto& Pair : EquippedItems)
	{
		if (Pair.Value != nullptr) return false;
	}

	return true;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEquipmentComponent::UpdateEquipmentMesh(EEquipmentSlot Slot, UEquipmentData* EquipmentData)
{
	EquipmentMeshes[Slot]->SetSkeletalMesh(nullptr);
	EquipmentMeshes[Slot]->SetSkeletalMesh(EquipmentData->EquipmentMesh);
}

void UEquipmentComponent::RemoveEquipmentMesh(EEquipmentSlot Slot)
{
	EquipmentMeshes[Slot]->SetSkeletalMesh(nullptr);
}

void UEquipmentComponent::AttachWeaponToSocket(UEquipmentData* WeaponData, FName SocketName)
{
	if (WeaponData->EquipmentMesh)
	{
		USkeletalMeshComponent* WeaponComp = EquipmentMeshes[WeaponData->EquipmentSlot];
		ABaseCharacter* Char = Cast<ABaseCharacter>(GetOwner());

		WeaponComp->AttachToComponent(
			Char->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			SocketName
		);

		return;
	}

	UE_LOG(LogTemp, Error, TEXT("Could not attatch weapon to hand socket"));
}

void UEquipmentComponent::InitEquipmentSlots()
{
	for (EEquipmentSlot Slot : TEnumRange<EEquipmentSlot>())
	{
		EquippedItems.Add(Slot, nullptr);
	}

	
}
