// Copyright (c) 2026 Andrew Las Casas. All rights reserved.


#include "RoomBuilderEditorSubsystem.h"
#include "Engine/World.h"

void URoomBuilderEditorSubsystem::Initialize(
    FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    CurrentLevelChangedHandle =
        FWorldDelegates::OnCurrentLevelChanged.AddUObject(
            this,
            &URoomBuilderEditorSubsystem::HandleCurrentLevelChanged
        );
}

void URoomBuilderEditorSubsystem::Deinitialize()
{
    FWorldDelegates::OnCurrentLevelChanged.Remove(
        CurrentLevelChangedHandle
    );

    Super::Deinitialize();
}

void URoomBuilderEditorSubsystem::HandleCurrentLevelChanged(
    ULevel* NewLevel,
    ULevel* OldLevel,
    UWorld* World)
{
    if (World && World->WorldType == EWorldType::Editor)
    {
        OnCurrentLevelChanged.Broadcast(NewLevel, OldLevel);
    }
}
