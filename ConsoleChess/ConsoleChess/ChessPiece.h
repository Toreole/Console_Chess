#pragma once
namespace ConsoleChess
{
	class ChessPiece
	{
	public: 
		int row;
		int column;

		//the character that represents this piece.
		virtual char GetCharacter() { return ' '; }

		//can this piece move to the targetted position?
		virtual bool CanMoveTo(int tr, int tc) { return false; }
	};
}