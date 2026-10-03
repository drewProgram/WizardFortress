#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RoomCellTypes.h"
#include "RoomBuilderEditorLibrary.generated.h"

class URoomLayoutData;

USTRUCT(BlueprintType)
struct FCellDataPair
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FIntPoint Key;

    UPROPERTY(BlueprintReadWrite)
    ERoomCellType Value;
};

USTRUCT(BlueprintType)
struct FCellGenerationData
{
    GENERATED_BODY()

    float WidthScale = 0.0f;
    float HeightScale = 0.0f;
    float DepthCm = 0.0f;
    float DepthScale = 0.0f;

    FVector Location = FVector::ZeroVector;
};

UCLASS()
class ROOMBUILDEREDITOR_API URoomBuilderEditorLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Room Builder")
    static void InspectLayout(const URoomLayoutData* Layout);

    UFUNCTION(BlueprintCallable, Category = "Room Builder")
    static bool GenerateRoom(
        const URoomLayoutData* Layout,
        FVector Origin,
        FName GenerationId
    );

    UFUNCTION(BlueprintCallable, Category = "Room Builder")
    static bool UpdateLayout(URoomLayoutData* Layout, const FCellDataPair& CellData);

    UFUNCTION(BlueprintCallable, Category = "Room Builder")
    static ERoomCellType ConvertStringToRoomCellType(const FString& Str);

private:
    static bool DefineBlockGenerationData(const TPair<FIntPoint, ERoomCellType>& CellData,
        const URoomLayoutData* Layout, const FVector& Origin, FCellGenerationData& CellGenerationData);
};