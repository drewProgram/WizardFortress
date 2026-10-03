#include "RoomBuilderEditorLibrary.h"
#include "RoomLayoutData.h"

#include "Components/StaticMeshComponent.h"
#include "Editor.h"
#include "Engine/Level.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "ScopedTransaction.h"
#include "Subsystems/EditorActorSubsystem.h"

void URoomBuilderEditorLibrary::InspectLayout(
    const URoomLayoutData* Layout)
{
    if (!IsValid(Layout))
    {
        UE_LOG(LogTemp, Warning, TEXT("Selecione um layout."));
        return;
    }

    UE_LOG(
        LogTemp,
        Display,
        TEXT("Sala: %d x %d | Celula: %.1f cm | Entradas: %d"),
        Layout->Width,
        Layout->Height,
        Layout->CellSize,
        Layout->Cells.Num()
    );

    for (auto& Cell : Layout->Cells)
    {
        if (!Layout->IsInsideGrid(Cell.Key))
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("Celula fora da grade: (%d, %d)"),
                Cell.Key.X,
                Cell.Key.Y
            );
        }
    }
}

bool URoomBuilderEditorLibrary::GenerateRoom(const URoomLayoutData* Layout, FVector Origin, FName GenerationId)
{
    if (!IsValid(Layout) || GenerationId.IsNone())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Informe um layout e um GenerationId."));
        return false;
    }

    if (Layout->Width <= 0 || Layout->Height <= 0
        || !FMath::IsFinite(Layout->CellSize)
        || Layout->CellSize <= 0.0f
        || Origin.ContainsNaN())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Dimensoes, tamanho da celula ou origem invalidos."));
        return false;
    }

    // Esta ferramenta opera com o jogo parado.
    if (!GEditor || GEditor->PlayWorld)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Encerre o Play/Simulate antes de gerar."));
        return false;
    }

    UWorld* World = GEditor->GetEditorWorldContext().World();

    if (!World || World->WorldType != EWorldType::Editor)
    {
        return false;
    }

    ULevel* Level = World->GetCurrentLevel();

    UEditorActorSubsystem* ActorSubsystem =
        GEditor->GetEditorSubsystem<UEditorActorSubsystem>();

    if (!Level || !ActorSubsystem)
    {
        return false;
    }

    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(
        nullptr,
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    if (!CubeMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Mesh do cubo nao encontrada."));
        return false;
    }

    // Validar todas as coordenadas antes de modificar o level.
    TMap<FIntPoint, FCellGenerationData> GenerationData;
    GenerationData.Reserve(Layout->Cells.Num());

    for (const TPair<FIntPoint, ERoomCellType>& Entry : Layout->Cells)
    {
        if (!Layout->IsInsideGrid(Entry.Key))
        {
            UE_LOG(LogTemp, Warning,
                TEXT("Celula fora da grade: (%d, %d). Geracao cancelada."),
                Entry.Key.X, Entry.Key.Y);

            return false;
        }

        FCellGenerationData CellData;

        if (!DefineBlockGenerationData(Entry, Layout, Origin, CellData))
        {
            UE_LOG(LogTemp, Error,
                TEXT("Dados de geracao invalidos na celula (%d, %d)."),
                Entry.Key.X, Entry.Key.Y);

            return false;
        }

        GenerationData.Add(Entry.Key, CellData);
    }

    // Duas tags identificam os Actors que esta ferramenta controla.
    const FName GeneratedTag(TEXT("RoomBuilder.Generated"));

    const FName IdTag(*FString::Printf(
        TEXT("RoomBuilder.Id.%s"),
        *GenerationId.ToString()
    ));

    TArray<AActor*> PreviousActors;

    // Somente o level atual, mesmo que outros estejam carregados.
    for (AActor* Actor : Level->Actors)
    {
        if (IsValid(Actor)
            && Actor->ActorHasTag(GeneratedTag)
            && Actor->ActorHasTag(IdTag))
        {
            PreviousActors.Add(Actor);
        }
    }

    // 2. A partir daqui, as alteracoes participam do Undo.
    const FScopedTransaction Transaction(
        NSLOCTEXT("RoomBuilder", "GenerateRoom", "Generate Room")
    );

    Level->Modify();

    // Coletamos antes para nao modificar a lista enquanto a percorremos.
    for (AActor* Actor : PreviousActors)
    {
        Actor->Modify();

        if (!ActorSubsystem->DestroyActor(Actor))
        {
            UE_LOG(LogTemp, Error,
                TEXT("Falha ao remover um bloco. Use Ctrl+Z para reverter."));
            return false;
        }
    }

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.OverrideLevel = Level;
    SpawnParameters.ObjectFlags |= RF_Transactional;
    SpawnParameters.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // 3. Criar os novos blocos.
    for (const TPair<FIntPoint, ERoomCellType>& Entry : Layout->Cells)
    {
        const FCellGenerationData& CellData = GenerationData[Entry.Key];

        AStaticMeshActor* Block = World->SpawnActor<AStaticMeshActor>(
            AStaticMeshActor::StaticClass(),
            CellData.Location,
            FRotator::ZeroRotator,
            SpawnParameters
        );

        if (!Block)
        {
            UE_LOG(LogTemp, Error,
                TEXT("Falha ao criar um bloco. Use Ctrl+Z para reverter."));
            return false;
        }

        Block->Modify();

        UStaticMeshComponent* MeshComponent =
            Block->GetStaticMeshComponent();

        MeshComponent->SetFlags(RF_Transactional);
        MeshComponent->Modify();
        MeshComponent->SetStaticMesh(CubeMesh);
        MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

        Block->SetActorScale3D(FVector(
            CellData.WidthScale,   // X: largura
            CellData.DepthScale,   // Y: profundidade
            CellData.HeightScale   // Z: altura
        ));

        Block->Tags.AddUnique(GeneratedTag);
        Block->Tags.AddUnique(IdTag);

        Block->SetActorLabel(FString::Printf(
            TEXT("RB_%s_%d_%d"),
            *GenerationId.ToString(),
            Entry.Key.X,
            Entry.Key.Y
        ));
    }

    Level->MarkPackageDirty();
    GEditor->RedrawLevelEditingViewports();

    UE_LOG(LogTemp, Display,
        TEXT("Geracao '%s': %d blocos criados."),
        *GenerationId.ToString(),
        Layout->Cells.Num());

    return true;
}

bool URoomBuilderEditorLibrary::UpdateLayout(URoomLayoutData* Layout, const FCellDataPair& CellData)
{
    if (!IsValid(Layout) || !Layout->IsInsideGrid(CellData.Key))
    {
        return false;
    }

    const FScopedTransaction Transaction(
        NSLOCTEXT("RoomBuilder", "UpdateRoomLayout", "Update Room Layout")
    );

    Layout->Modify(); // Antes de alterar Cells.

    if (const ERoomCellType* Type = Layout->Cells.Find(CellData.Key))
    {
        if (*Type == CellData.Value)
        Layout->Cells.Remove(CellData.Key);
        Layout->MarkPackageDirty();
        return false; // Agora est� vazia.
    }

    TPair<FIntPoint, ERoomCellType> CellDataPair(CellData.Key, CellData.Value);
    Layout->Cells.Add(CellDataPair);
    Layout->MarkPackageDirty();
    return true; // Agora est� s�lida.
}

ERoomCellType URoomBuilderEditorLibrary::ConvertStringToRoomCellType(const FString& Str)
{
    const UEnum* Enum = StaticEnum<ERoomCellType>();

    for (int32 Index = 0; Index < Enum->NumEnums(); ++Index)
    {
        if (Enum->GetDisplayNameTextByIndex(Index).ToString() == Str)
        {
            return static_cast<ERoomCellType>(Enum->GetValueByIndex(Index));
        }
    }

    return ERoomCellType::None;
}

bool URoomBuilderEditorLibrary::DefineBlockGenerationData(const TPair<FIntPoint, ERoomCellType>& Entry, const URoomLayoutData* Layout, const FVector& Origin, FCellGenerationData& CellGenerationData)
{
    switch (Entry.Value)
    {
    case ERoomCellType::BasicStructure:
        CellGenerationData.WidthScale = Layout->CellSize / 100.0f;
        CellGenerationData.HeightScale = Layout->CellSize / 100.0f;
        CellGenerationData.DepthCm = Layout->Depth * Layout->CellSize;
        CellGenerationData.DepthScale = CellGenerationData.DepthCm / 100.0f;

        CellGenerationData.Location = Origin + FVector(
            (Entry.Key.X + 0.5f) * Layout->CellSize,
            Origin.Y * -0.5,
            (Entry.Key.Y + 0.5f) * Layout->CellSize
        );

        break;
    
    default:
        return false;
    }
    return true;
}
