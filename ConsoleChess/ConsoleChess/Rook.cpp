#include "Rook.h"
#include <cmath>
#include <iostream>

using namespace ConsoleChess;

bool ConsoleChess::Rook::CanMoveTo(int tx, int ty, std::array<std::array<ChessPiece*, 8>, 8>* board)
{
    int dx = tx - row;
    int dy = ty - column;

    int adx = std::abs(dx);
    int ady = std::abs(dy);

    if ((dx == 0 && dy != 0) || (dy == 0 && dx != 0))
    {
        int xDir = dx > 0 ? 1 : dx < 0 ? -1 : 0;
        int yDir = dy > 0 ? 1 : dy < 0 ? -1 : 0;

        int xx = row + xDir;
        int yy = column + yDir;

        //all positions between the start and the destination.
        for (; xx != tx || yy != ty; xx += xDir, yy += yDir)
        {
            ChessPiece* other = board->at(xx).at(yy);
            if (other != nullptr)
            {
                std::cout << "piece in the way." << std::endl;
                return false;
            }
        }
        //the final position.
        ChessPiece* other = board->at(tx).at(yy);
        if (other != nullptr)
        {
            std::cout << "cant take ally" << std::endl;
            return other->color != color;
        }
        return true;
    }
    else
    {
        std::cout << "not a linear move: " << adx << " | " << ady << std::endl;
    }
    return false;
}
