#include "Pawn.h"
#include <cmath>
#include <iostream>

using namespace ConsoleChess;

bool Pawn::CanMoveTo(int tx, int ty, std::array<std::array<ChessPiece*, 8>, 8>* board)
{
	//vertical movement only for now.
	//validate the direction.
	int deltaY = ty - column;
	int deltaX = tx - row;

	if (deltaX == 0)
	{
		std::cout << "Reason: cant move sideways" << std::endl;
		return false;
	}

	//color 0 can only move in positive X direction. 1 only in negative X. 0 is never allowed.
	bool legalX = (color == 0) ? deltaX > 0 : deltaX < 0;
	if (!legalX)
	{
		std::cout << "Reason: wrong vertical direction" << std::endl;
		return false;
	}

	//check absolute deltaX.
	if (std::abs(deltaX) == 2)
	{
		int midX = row + (deltaX / 2);
		if (board->at(midX).at(column) != nullptr)
		{
			std::cout << "There was a piece blocking the path." << std::endl;
			return false;
		}
		//double move only allowed if still on the starting position.
		if (row != startRow || column != startColumn)
		{
			std::cout << "Reason: cant double when not in starting position." << std::endl;
			return false;
		}
		//diagonal and 2 front is never allowed.
		if (deltaY != 0)
		{
			std::cout << "Reason: cant move sideways." << std::endl;
			return false;
		}
	}
	//moving more than 2 is never allowed.
	else if (std::abs(deltaX) > 2)
	{
		std::cout << "Reason: cant move more than 2 tiles." << std::endl;
		return false;
	}
	//deltaY can at max be 1.
	if (std::abs(deltaY) > 1)
	{
		std::cout << "Reason: cant move sideways more than 1 tile." << std::endl;
		return false;
	}
	//check the field.
	ChessPiece* otherPiece = board->at(tx).at(ty);
	if (otherPiece != nullptr)
	{
		//can never take a piece of the same color.
		if (otherPiece->color == color)
		{
			std::cout << "Reason: cant take ally piece." << std::endl;
			return false;
		}
		//pawns can never take by just walking vertically.
		if (deltaY == 0)
		{
			std::cout << "Reason: cant take frontal." << std::endl;
			return false;
		}
	}
	//other field is empty
	if (deltaY > 0)
	{
		std::cout << "Reason: cant walk diagonally for no reason." << std::endl;
		return false;
	}
	return true;
}