#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoomCellTypes.h"
#include "RoomLayoutData.generated.h"



UCLASS(BlueprintType)
class ROOMBUILDER_API URoomLayoutData : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room",
        meta = (ClampMin = "1"))
    int32 Width = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room",
        meta = (ClampMin = "1"))
    int32 Height = 7;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room",
        meta = (ClampMin = "1"))
    int32 Depth = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room",
        meta = (ClampMin = "1.0", Units = "cm"))
    float CellSize = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
    TMap<FIntPoint, ERoomCellType> Cells;

    UFUNCTION(BlueprintPure, Category = "Room")
    bool IsInsideGrid(FIntPoint Cell) const;
};