#include "King.h"
#include "Board.h"
#include <cmath>
#include <iostream>

bool ConsoleChess::King::CanMoveTo(int tx, int ty, Board* board, bool printIssues)
{
    int deltaX = tx - row;
    int deltaY = ty - column;
    int ady = std::abs(deltaY);

    if (std::abs(deltaX) <= 1 && ady <= 1)
    {
        ChessPiece* other = board->GetPieceAt(tx, ty);
        if (other != nullptr)
        {
            if (other->color == color)
            {
                PRINTIF("can not take ally.", printIssues);
                return false;
            }
        }
        return true;
    } 
    //if moving two places to the side, this is a castle move.
    if (deltaX == 0 && ady == 2)
    {
        int direction = deltaY > 0 ? 1 : -1;
        bool castleAllowed = board->CanCastle(this, direction);
        if (castleAllowed)
        {
            int yy = (direction == 1) ? 7 : 0;
            ChessPiece* rook = board->GetPieceAt(row, yy); 
            rook->hasMoved = true;
            rook->column = column + direction;
            board->SetPieceAt(nullptr, row, yy);
            board->SetPieceAt(rook, rook->row, rook->column);
        }
        else
        {
            PRINTIF("can not castle like this.", printIssues);
        }
        return castleAllowed;
    }
    PRINTIF("invalid king move.", printIssues);
    return false;
}
