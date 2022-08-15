#pragma once
#include "ChessPiece.h"
namespace ConsoleChess
{
    class Knight : public ChessPiece
    {
    public:
        Knight(int r, int c, int col) { row = r; column = c; color = col; startRow = row; startColumn = column; }
        Knight() { row = 0; column = 0; color = 0; }
        char GetCharacter() { return 'N'; }
        bool CanMoveTo(int tr, int tc, Board* board, bool printIssues = true);
    };
}