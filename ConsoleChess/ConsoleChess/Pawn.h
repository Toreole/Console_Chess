#pragma once
#include "ChessPiece.h"

namespace ConsoleChess
{
    class Pawn : public ChessPiece
    {
    public:
        Pawn(int r, int c, int col) { row = r; column = c; color = col; startRow = row; startColumn = column; }
        Pawn() { row = 0; column = 0; color = 0; }
        char GetCharacter() { return 'P'; }

        bool CanMoveTo(int tx, int ty, Board* board, bool printIssues = true);
    };
}