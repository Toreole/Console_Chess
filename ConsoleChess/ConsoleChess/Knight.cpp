#include "Knight.h"
#include "Board.h"
#include <cmath>

bool ConsoleChess::Knight::CanMoveTo(int tx, int ty, Board* board, bool printIssues)
{
    int dx = tx - row;
    int dy = ty - column;
    int adx = std::abs(dx);
    int ady = std::abs(dy);

    if (adx == 2 && ady == 1 || adx == 1 && ady == 2)
    {
        ChessPiece* other = board->GetPieceAt(tx, ty);
        if (other != nullptr)
        {
            if (other->color == color)
                return false;
        }
        return true;
    }
    return false;
}
