#include "Board.h"
#include "Rook.h"
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
	board[0][0] = new Rook();
}

void Board::Render()
{
	SetConsoleTextAttribute(console_handle, color_blue);
	std::cout << board[0][0]->GetCharacter();
}

void Board::Reset()
{

}