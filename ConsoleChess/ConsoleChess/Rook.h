#pragma once
#include "ChessPiece.h"
namespace ConsoleChess
{
    class Rook : public ChessPiece
    {
    public:
        Rook(int r, int c, int col) { row = r; column = c; color = col; startRow = row; startColumn = column; }
        Rook() { row = 0; column = 0; color = 0; }
        char GetCharacter() { return 'R'; }
    };
}