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
		std::array<ChessPiece*, 10> testBuffer; //outside of custom games there is no way to get 10 pieces of the same type anyway.

		//the king of player 0
		King* playerA_King = nullptr;
		//the king of player 1
		King* playerB_King = nullptr;

		std::string intToStringCoordinates(int, int);

		int findOtherPiecesToMoveTo(int x, int y, ChessPiece* original);

		bool moveCausesCheckOnSelf(int x, int y, ChessPiece* moved);

		/// <summary>
		/// Gets an unambiguous string representing the move according to the algebraic notation standard.
		/// WARNING: Currently does not work with pawn promotions.
		/// </summary>
		/// <param name="ax">start x of the moved piece</param>
		/// <param name="ay">start y of the moved piece</param>
		/// <param name="bx">target x of the moved piece</param>
		/// <param name="by">target y of the moved piece</param>
		/// <param name="piece">the moved piece</param>
		/// <param name="other">the piece at bx/by</param>
		/// <param name="ambiguousCount">the count of ambiguous moves, as given by findOtherPiecesToMoveTo()</param>
		/// <param name="takingPiece">other!=nullptr</param>
		/// <returns>the algebraic notation for the performed move.</returns>
		std::string getDisambiguatedMoveNotation(int ax, int ay, int bx, int by, ChessPiece* piece, ChessPiece* other, int ambiguousCount, bool takingPiece);

#define KING_OF(color) color == 1 ? playerB_King : playerA_King;  

	public:
		//draw the board in the console.
		void Render();

		//reset the board.
		void Reset();

		//initialize the board.
		void Initialize();

		//initializes the board according to a description.
		//fomat: [piece][colour] [piece][colour] ... 64 entries expected.
		//x = no piece, P = pawn, Q = Queen, K = King, B = Bishop, N = Knight, R = Rook | colour is either 0 or 1
		void Initialize(std::string& boardDescription);

		//try to make a specific move.
		bool TryMakeMove(ChessMove* move, int player);

		//Force a move regardless of conditions. used for replays.
		void ForceMove(ChessMove* m, int p);

		//gets the pointer to the piece at the location.
		ChessPiece* GetPieceAt(int x, int y){ return board[x][y]; }

		void SetPieceAt(ChessPiece* piece, int x, int y) { board[x][y] = piece; }

		bool IsTileAttacked(int x, int y, int player);

		//can the specified king castle to the given side?
		bool CanCastle(King*, int);

		//default constructor
		Board();

		//destructor
		~Board();
	};
}