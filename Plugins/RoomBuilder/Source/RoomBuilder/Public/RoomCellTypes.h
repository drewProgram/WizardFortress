#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ERoomCellType : uint8
{
    None,
    BasicStructure,
    // Futuramente: Door, Platform...
};