#include "ChessPiece.h"
#include <iostream>

bool ConsoleChess::ChessPiece::HasLineOfSightTo(int tx, int ty, int xStep, int yStep, std::array<std::array<ChessPiece*, 8>, 8>* board)
{
    //the x and y to check. start at the next position after step.
    int xx = row + xStep; int yy = column + yStep;

    //step forward, and check every position on the way to the target position.
    for (; xx != tx || yy != ty; xx += xStep, yy += yStep)
    {
        //if any piece blocks the path, return false.
        if (board->at(xx).at(yy) != nullptr)
        {
            std::cout << "Piece blocking the path." << std::endl;
            return false;
        }
    }
    return true;
}
