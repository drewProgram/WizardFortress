// Copyright (c) 2026 Andrew Las Casas. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "RoomBuilderEditorSubsystem.generated.h"

class ULevel;
class UWorld;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FRoomBuilderCurrentLevelChanged,
    ULevel*, NewLevel,
    ULevel*, OldLevel
);

UCLASS()
class ROOMBUILDEREDITOR_API URoomBuilderEditorSubsystem
    : public UEditorSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UPROPERTY(BlueprintAssignable, Category = "Room Builder")
    FRoomBuilderCurrentLevelChanged OnCurrentLevelChanged;

private:
    void HandleCurrentLevelChanged(
        ULevel* NewLevel,
        ULevel* OldLevel,
        UWorld* World
    );

    FDelegateHandle CurrentLevelChangedHandle;
};
