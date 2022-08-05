#pragma once
#include <array>

namespace ConsoleChess
{
	class ChessPiece
	{
	protected:
		//this is protected because the xStep and yStep are defined by the pieces themselves.
		
		//Checks if the piece has a line of sight to the target position (tx, ty)
		bool HasLineOfSightTo(int tx, int ty, int xStep, int yStep, std::array<std::array<ChessPiece*, 8>, 8>* board);

	public:
		int startRow = 0, startColumn = 0;
		int row = 0;
		int column = 0;
		int color = 0;

		ChessPiece(int r, int c, int col) { row = r; column = c; color = col; startRow = row; startColumn = column; }
		ChessPiece() { row = 0; column = 0; color = 0; }

		//the character that represents this piece.
		virtual char GetCharacter() { return ' '; }

		//can this piece move to the targetted position?
		virtual bool CanMoveTo(int tr, int tc, std::array<std::array<ChessPiece*, 8>, 8>* board, bool printIssues = true) { return false; }

	};
}

#define PRINTIF(ln, prb) if(prb) std::cout << ln << std::endl;