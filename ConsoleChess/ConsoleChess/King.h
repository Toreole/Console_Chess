#pragma once
#include "ChessPiece.h"
namespace ConsoleChess
{
    class King : public ChessPiece
    {
    public:
        King(int r, int c, int col) { row = r; column = c; color = col; startRow = row; startColumn = column; }
        King() { row = 0; column = 0; color = 0; }
        char GetCharacter() { return 'K'; }
        bool CanMoveTo(int tx, int ty, Board* board, bool printIssues = true);
    };
}