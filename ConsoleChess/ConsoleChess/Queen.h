#pragma once
#include "ChessPiece.h"
namespace ConsoleChess
{
	class Queen : public ChessPiece
	{
	public:
		Queen(int r, int c, int col) { row = r; column = c; color = col; startRow = row; startColumn = column; }
		Queen() { row = 0; column = 0; color = 0; }
		char GetCharacter() { return 'Q'; }
	};
}