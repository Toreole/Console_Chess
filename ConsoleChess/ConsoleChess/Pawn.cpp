#include "Pawn.h"
#include "Board.h"
#include <cmath>
#include <iostream>

using namespace ConsoleChess;

bool Pawn::CanMoveTo(int tx, int ty, Board* board, bool printIssues)
{
	//vertical movement only for now.
	//validate the direction.
	int deltaY = ty - column;
	int deltaX = tx - row;

	if (deltaX == 0)
	{
		PRINTIF("Reason: cant move sideways", printIssues);
		return false;
	}

	//color 0 can only move in positive X direction. 1 only in negative X. 0 is never allowed.
	bool legalX = (color == 0) ? deltaX > 0 : deltaX < 0;
	if (!legalX)
	{
		PRINTIF("Reason: wrong vertical direction", printIssues);
		return false;
	}

	//check absolute deltaX.
	if (std::abs(deltaX) == 2)
	{
		int midX = row + (deltaX / 2);
		if (board->GetPieceAt(midX, column) != nullptr)
		{
			PRINTIF("There was a piece blocking the path.", printIssues);
			return false;
		}
		//double move only allowed if still on the starting position.
		if (row != startRow || column != startColumn)
		{
			PRINTIF("Reason: cant double when not in starting position.", printIssues);
			return false;
		}
		//diagonal and 2 front is never allowed.
		if (deltaY != 0)
		{
			PRINTIF("Reason: cant move sideways.", printIssues);
			return false;
		}
	}
	//moving more than 2 is never allowed.
	else if (std::abs(deltaX) > 2)
	{
		PRINTIF("Reason: cant move more than 2 tiles.", printIssues);
		return false;
	}
	//deltaY can at max be 1.
	if (std::abs(deltaY) > 1)
	{
		PRINTIF("Reason: cant move sideways more than 1 tile.", printIssues);
		return false;
	}
	//check the field.
	ChessPiece* otherPiece = board->GetPieceAt(tx, ty);
	if (otherPiece != nullptr)
	{
		//can never take a piece of the same color.
		if (otherPiece->color == color)
		{
			PRINTIF("Reason: cant take ally piece.", printIssues);
			return false;
		}
		//pawns can never take by just walking vertically.
		if (deltaY == 0)
		{
			PRINTIF("Reason: cant take frontal.", printIssues);
			return false;
		}
		//should be able to take pieces like this.
		if (std::abs(deltaY) == 1)
		{
			return true;
		}
	}
	//other field is empty
	if (deltaY != 0)
	{
		PRINTIF("Reason: cant walk diagonally for no reason.", printIssues);
		return false;
	}
	return true;
}