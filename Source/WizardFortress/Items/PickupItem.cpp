#include "Items/PickupItem.h"

#include "Components/BoxComponent.h"

#include "Characters/PlayerCharacter.h"
#include "Systems/Inventory/InventoryComponent.h"

APickupItem::APickupItem()
{
	//PrimaryActorTick.bCanEverTick = true;

	ItemData = nullptr;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	RootComponent = CollisionBox;

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	WorldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("World Mesh"));
	WorldMesh->SetupAttachment(RootComponent);
}

void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	if (ItemData->WorldMesh)
	{
		WorldMesh->SetStaticMesh(ItemData->WorldMesh);
	}
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::HandleBeginOverlap);
}

void APickupItem::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(OtherActor))
		{
			UInventoryComponent* Inventory = PlayerActor->GetInventoryComponent();
			if (Inventory->CanAddItem(ItemData))
			{
				Destroy();
				return;
			}

			UE_LOG(LogTemp, Warning, TEXT("Could not add item to inventory!"));
		}
	}
}
