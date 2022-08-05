#include "King.h"
#include <cmath>

bool ConsoleChess::King::CanMoveTo(int tx, int ty, std::array<std::array<ChessPiece*, 8>, 8>* board, bool printIssues)
{
    int deltaX = tx - row;
    int deltaY = ty - column;

    if (std::abs(deltaX) <= 1 && std::abs(deltaY) <= 1)
    {
        ChessPiece* other = board->at(tx).at(ty);
        if (other != nullptr)
        {
            if (other->color == color)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
