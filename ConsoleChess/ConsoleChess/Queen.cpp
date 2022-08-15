#include "Queen.h"
#include "Board.h"
#include <cmath>
#include <iostream>

bool ConsoleChess::Queen::CanMoveTo(int tx, int ty, Board* board, bool printIssues)
{
    int dx = tx - row;
    int dy = ty - column;

    int adx = std::abs(dx);
    int ady = std::abs(dy);

    //diagonal movement from bishop.
    if (adx == ady && adx != 0)
    {
        int xDir = dx > 0 ? 1 : -1;
        int yDir = dy > 0 ? 1 : -1;

        if (!HasLineOfSightTo(tx, ty, xDir, yDir, board))
            return false;

        //the final position.
        ChessPiece* other = board->GetPieceAt(tx, ty);
        if (other != nullptr)
        {
            PRINTIF("cant take ally", color == other->color && printIssues);
            return other->color != color;
        }
        return true;
    }
    //linear movement from Rook
    if ((dx == 0 && dy != 0) || (dy == 0 && dx != 0))
    {
        int xDir = dx > 0 ? 1 : dx < 0 ? -1 : 0;
        int yDir = dy > 0 ? 1 : dy < 0 ? -1 : 0;

        if (!HasLineOfSightTo(tx, ty, xDir, yDir, board))
            return false;

        //the final position.
        ChessPiece* other = board->GetPieceAt(tx, ty);
        if (other != nullptr)
        {
            PRINTIF("cant take ally", color == other->color && printIssues);
            return other->color != color;
        }
        return true;
    }
    else
    {
        PRINTIF("not a valid move for queen", printIssues);
    }
    return false;
}
