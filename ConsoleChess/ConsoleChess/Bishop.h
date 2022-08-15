#pragma once
#include "ChessPiece.h"
namespace ConsoleChess
{
    class Bishop : public ChessPiece
    {
    public:
        Bishop(int r, int c, int col) { row = r; column = c; color = col; startRow = row; startColumn = column; }
        Bishop() { row = 0; column = 0; color = 0; }
        char GetCharacter() { return 'B'; }
        bool CanMoveTo(int tx, int ty, Board* board, bool printIssues = true);
    };
}