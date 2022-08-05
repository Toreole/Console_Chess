#include "Rook.h"
#include <cmath>
#include <iostream>

using namespace ConsoleChess;

bool ConsoleChess::Rook::CanMoveTo(int tx, int ty, std::array<std::array<ChessPiece*, 8>, 8>* board, bool printIssues)
{
    int dx = tx - row;
    int dy = ty - column;

    int adx = std::abs(dx);
    int ady = std::abs(dy);

    if ((dx == 0 && dy != 0) || (dy == 0 && dx != 0))
    {
        int xDir = dx > 0 ? 1 : dx < 0 ? -1 : 0;
        int yDir = dy > 0 ? 1 : dy < 0 ? -1 : 0;

        //all positions between the start and the destination.
        if (!HasLineOfSightTo(tx, ty, xDir, yDir, board))
            return false;

        //the final position.
        ChessPiece* other = board->at(tx).at(ty);
        if (other != nullptr)
        {
            PRINTIF("cant take ally", color == other->color && printIssues);
            return other->color != color;
        }
        return true;
    }
    else
    {
        PRINTIF("not a linear move", printIssues);
    }
    return false;
}
