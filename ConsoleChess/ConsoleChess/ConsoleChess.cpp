// ConsoleChess.cpp : startup.

#include <iostream>
#include "Board.h"

using namespace ConsoleChess;

int main()
{
    std::cout << "Hello World!\n";

    std::string input;

    //set up everything.
    std::unique_ptr<Board> board(new Board());
    board->Initialize();
    board->Render();
    return 0;
}
