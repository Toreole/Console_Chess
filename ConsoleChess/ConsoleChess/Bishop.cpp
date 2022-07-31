#include "Bishop.h"
#include <cmath>
#include <iostream>

bool ConsoleChess::Bishop::CanMoveTo(int tx, int ty, std::array<std::array<ChessPiece*, 8>, 8>* board)
{
    int dx = tx - row;
    int dy = ty - column;

    int adx = std::abs(dx);
    int ady = std::abs(dy);

    if (adx == ady && adx != 0)
    {
        int xDir = dx > 0 ? 1 : -1;
        int yDir = dy > 0 ? 1 : -1;

        if (!HasLineOfSightTo(tx, ty, xDir, yDir, board))
            return false;

        //the final position.
        ChessPiece* other = board->at(tx).at(ty);
        if (other != nullptr)
        {
            std::cout << "cant take ally" << std::endl;
            return other->color != color;
        }
        return true;
    }
    else
    {
        std::cout << "not a diagonal move: " << adx << " | " << ady << std::endl;
    }
    return false;
}
