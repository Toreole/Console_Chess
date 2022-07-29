#include "Board.h"
#include "ChessPieces.h"
#include <iostream>
#include <Windows.h>

using namespace ConsoleChess;

static int color_blue = 11;
static int color_green = 10;
static int color_white = 7;

void Board::Initialize()
{
	//get the handle for the console.
	console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	//set up the board.
	//"white" side.
	board[0][0] = new Rook(0, 0, 0);
	board[0][1] = new Knight(0, 1, 0);
	board[0][2] = new Bishop(0, 2, 0);
	board[0][3] = new Queen(0, 3, 0);
	board[0][4] = new King(0, 4, 0);
	board[0][5] = new Bishop(0, 5, 0);
	board[0][6] = new Knight(0, 6, 0);
	board[0][7] = new Rook(0, 7, 0);

	for (int i = 0; i <= 7; ++i)
		board[1][i] = new Pawn(1, i, 0);

	//"black" side
	board[7][0] = new Rook(7, 0, 1);
	board[7][1] = new Knight(7, 1, 1);
	board[7][2] = new Bishop(7, 2, 1);
	board[7][3] = new Queen(7, 3, 1);
	board[7][4] = new King(7, 4, 1);
	board[7][5] = new Bishop(7, 5, 1);
	board[7][6] = new Knight(7, 6, 1);
	board[7][7] = new Rook(7, 7, 1);

	for (int i = 0; i <= 7; ++i)
		board[6][i] = new Pawn(6, i, 1);
}

void Board::Render()
{
	int color = true ? color_blue : color_green;
	
	const std::string letters = "abcdefgh";

	std::cout << "------------------" << std::endl;

	for (int i = 0; i <= 7; ++i)
		std::cout << letters[i] << ' ';
	std::cout << std::endl;

	//draw all the things.
	for (int x = 0; x <= 7; ++x)
	{
		for (int y = 0; y <= 7; ++y)
		{
			//skip nulls
			if (board[x][y] == nullptr)
			{
				std::cout << "  ";
				continue;
			}
			//set the color, then draw the character.
			SetConsoleTextAttribute(console_handle, board[x][y]->color == 1 ? color_blue : color_green);
			std::cout << board[x][y]->GetCharacter() << ' ';
		}
		SetConsoleTextAttribute(console_handle, color_white);
		std::cout << (x+1) << std::endl;
	}

}

void Board::Reset()
{
	//remove all pieces from the board.
	for (int x = 0; x <= 7; ++x)
	{
		for (int y = 0; y <= 7; ++y)
		{
			ChessPiece* piece = board[x][y];
			if (piece != nullptr)
			{
				board[x][y] = nullptr;
				takenPieces.push_back(piece);
			}
		}
	}
	//put all pieces in their original positions.
	for (int i = 0; i < takenPieces.size(); ++i)
	{
		ChessPiece* piece = takenPieces.at(i);
		board[piece->startRow][piece->startColumn] = piece;
	}
}

bool Board::TryMakeMove(int ax, int ay, int bx, int by, int player)
{
	ChessPiece* piece = board[ax][ay];
	//validate that a non-empty field was selected.
	if (piece == nullptr)
	{
		std::cout << "Selected Piece was null." << std::endl;
		return false;
	}
	std::cout << "Selected Piece: " << piece->GetCharacter() << std::endl;
	//validate that this piece belongs to the player
	if (player != piece->color)
	{
		std::cout << "Selected Piece belonged to other player." << std::endl;
		return false;
	}
	//validate that the piece can move to that position
	if (!piece->CanMoveTo(bx, by, &board))
	{
		std::cout << "Selected Piece cant move to target position." << std::endl;
		return false;
	}
	//Move the piece. automatically "take" other pieces in the spot
	ChessPiece* other = board[bx][by];
	if (other != nullptr)
	{
		//put the piece in the taken vector.
		takenPieces.push_back(other);
	}
	//place the moved piece in the second spot.
	board[bx][by] = piece;
	//update the position of the piece itself
	piece->row = bx;
	piece->column = by;

	//clear the spot where it was before
	board[ax][ay] = nullptr;
	return true;
}