// Copyright © 2026 Andrew Las Casas. All rights reserved.
#include "RoomOrigin.h"

#include "Components/ArrowComponent.h"

// Sets default values
ARoomOrigin::ARoomOrigin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    DirectionArrow = CreateDefaultSubobject<UArrowComponent>(
        TEXT("DirectionArrow")
    );
    DirectionArrow->SetupAttachment(RootComponent);

    DirectionArrow->SetArrowColor(FLinearColor::Red);
    DirectionArrow->SetArrowSize(1.5f);
    DirectionArrow->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void ARoomOrigin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomOrigin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

