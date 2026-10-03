#include "RoomLayoutData.h"

bool URoomLayoutData::IsInsideGrid(FIntPoint Cell) const
{
    return Cell.X >= 0 && Cell.X < Width
        && Cell.Y >= 0 && Cell.Y < Height;
}