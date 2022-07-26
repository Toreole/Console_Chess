#pragma once

#include <array>
#include "ChessPiece.h"
#include <Windows.h> //required to get the handle of the console window.

namespace ConsoleChess
{
	//! class that defines a chess board.
	class Board
	{
	private:
		//should functionally be a 2D "array" of sorts.
		std::array<std::array<ChessPiece*, 8>, 8> board;
		HANDLE console_handle;

	public:
		//draw the board in the console.
		void Render();

		//reset the board.
		void Reset();

		//initialize the board.
		void Initialize();
	};
}