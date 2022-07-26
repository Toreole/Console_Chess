#pragma once
#include "ChessPiece.h"
namespace ConsoleChess
{
    class Rook : public ChessPiece
    {
    public:
        char GetCharacter() { return 'R'; }
    };
}