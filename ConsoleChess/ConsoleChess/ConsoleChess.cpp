// ConsoleChess.cpp : startup.

#include <iostream>
#include "Board.h"
#include <regex>

using namespace ConsoleChess;

int intFromChar(char c)
{
    switch (c)
    {
    case 'a':
    case '1':
        return 0;
    case 'b':
    case '2':
        return 1;
    case 'c':
    case '3':
        return 2;
    case 'd':
    case '4':
        return 3;
    case 'e':
    case '5':
        return 4;
    case 'f':
    case '6':
        return 5;
    case 'g':
    case '7':
        return 6;
    case 'h':
    case '8':
        return 7;
    default:
        return 0;
    }
}

int main()
{
    std::cout << "Hello World!\n";

    std::string a, b;
    std::string input;

    //set up everything.
    std::unique_ptr<Board> board(new Board());
    board->Initialize();

    int player = 0;
    //this defines valid input. 
    std::regex rgxPattern("([a-h][1-8])\\s([a-h][1-8])");


    while (true)
    {
        board->Render();

        //prompt input:
        std::cout << "Make your move: " << std::endl;
        //get input
        
        std::cin >> a >> b;
        input = a + " " + b;

        if (input.size() < 5)
        {
            std::cout << "INPUT TOO SHORT, WAS SIZE: " << input.size() << std::endl;
            continue;
        }
        //parse input.
        bool validInput = std::regex_match(input, rgxPattern);

        //this is a little scuffed and mixed up because i did something silly with the setup of the board, so it has to be like this.

        int ay = intFromChar(input[0]);
        int ax = intFromChar(input[1]);
        //input[2] should be white space.
        int by = intFromChar(input[3]);
        int bx = intFromChar(input[4]);

        //std::cout << "TRIED TO MOVE FROM: " << ax << ", " << ay << " TO " << bx << ", " << by << std::endl;

        if (board->TryMakeMove(ax, ay, bx, by, player))
        {
            //switch between 0 and 1.
            player = 1 - player;
            continue;
        }
        else
        {
            std::cout << "INVALID MOVE" << std::endl;
        }
    }

    return 0;
}
