#pragma once

#include <array>
#include <vector>
#include <Windows.h> //required to get the handle of the console window.
#include "ChessPiece.h"

namespace ConsoleChess
{
	//! class that defines a chess board.
	class Board
	{
	private:
		//should functionally be a 2D "array" of sorts.
		std::array<std::array<ChessPiece*, 8>, 8> board;
		HANDLE console_handle;
		std::vector<ChessPiece*> takenPieces;

	public:
		//draw the board in the console.
		void Render();

		//reset the board.
		void Reset();

		//initialize the board.
		void Initialize();

		//try to make a specific move.
		bool TryMakeMove(int ax, int ay, int bx, int by, int player);

		ChessPiece* GetPieceAt(int x, int y){ return board[x][y]; }
	};
}