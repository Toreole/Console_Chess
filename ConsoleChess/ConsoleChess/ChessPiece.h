#pragma once

namespace ConsoleChess
{
	class ChessPiece
	{
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
		virtual bool CanMoveTo(int tr, int tc, ChessPiece* otherPiece) { return false; }
	};
}