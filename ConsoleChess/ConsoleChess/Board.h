#pragma once

#include <array>
#include <vector>
#include <Windows.h> //required to get the handle of the console window.
#include "ChessPieces.h"
#include "ChessMove.h"

namespace ConsoleChess
{
	//class that defines a chess board.
	class Board
	{
	private:
		//should functionally be a 2D "array" of sorts.
		std::array<std::array<ChessPiece*, 8>, 8> board;
		HANDLE console_handle;
		std::vector<ChessPiece*> takenPieces;

		//the king of player 0
		King* playerA_King = nullptr;
		//the king of player 1
		King* playerB_King = nullptr;

	public:
		//draw the board in the console.
		void Render();

		//reset the board.
		void Reset();

		//initialize the board.
		void Initialize();

		//try to make a specific move.
		bool TryMakeMove(ChessMove* move, int player);

		//Force a move regardless of conditions. used for replays.
		void ForceMove(ChessMove& m, int p);

		//gets the pointer to the piece at the location.
		ChessPiece* GetPieceAt(int x, int y){ return board[x][y]; }

		void SetPieceAt(ChessPiece* piece, int x, int y) { board[x][y] = piece; }

		bool IsTileAttacked(int x, int y, int player);

		//can the specified king castle to the given side?
		bool CanCastle(King*, int);
	};
}