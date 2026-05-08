#include "Characters/BaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Systems/Inventory/InventoryComponent.h"
#include "Systems/Inventory/EquipmentComponent.h"
#include "Systems/Inventory/InventoryTypes.h"
#include "Systems/Inventory/ConsumableManager.h"
#include "Items/EquipmentData.h"
#include "Systems/AttributeSystem.h"
#include "Combat/SkillManager.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipments"));
	SkillManager = CreateDefaultSubobject<USkillManager>(TEXT("Skills"));
	AttributeSystem = CreateDefaultSubobject<UAttributeSystem>(TEXT("Attributes"));
	ConsumableManager = CreateDefaultSubobject<UConsumableManager>(TEXT("Consumables Manager"));

	SkillSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Skill Spawn Point"));
	SkillSpawnPoint->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

UInventoryComponent* ABaseCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

UEquipmentComponent* ABaseCharacter::GetEquipmentComponent() const
{
	return EquipmentComponent;
}

USkillManager* ABaseCharacter::GetSkillComponent() const
{
	return SkillManager;
}

UAttributeSystem* ABaseCharacter::GetAttributeComponent() const
{
	return AttributeSystem;
}

UConsumableManager* ABaseCharacter::GetConsumableComponent() const
{
	return ConsumableManager;
}

USceneComponent* ABaseCharacter::GetSkillSpawnPoint() const
{
	return SkillSpawnPoint;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = GetAttributeComponent()->GetMovementSpeed();
	GetAttributeComponent()->OnCharacterDied.AddUObject(this, &ABaseCharacter::HandleDeath);

	if (EquipmentComponent->AreEquipmentsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("No equipments equipped!"));
		ApplyDefaultEquipment();
	}
}

void ABaseCharacter::ApplyDefaultEquipment()
{
	for (const TPair<EEquipmentSlot, UEquipmentData*>& Pair : DefaultEquipments)
	{
		const EEquipmentSlot Slot = Pair.Key;
		UEquipmentData* ItemData = Pair.Value;

		if (!ItemData) continue;

		// adicionar itens ao inventário
		bool HasAddedItem = InventoryComponent->CanAddItem(Cast<UItemData>(ItemData));
		if (HasAddedItem)
		{
			// equipar itens
			UE_LOG(LogTemp, Display, TEXT("Added item to inventory"));
			EquipmentComponent->EquipItem(ItemData);

			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("Could not add default item to inventory"));
	}
}

void ABaseCharacter::HandleDeath()
{
	Destroy();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
