// ConsoleChess.cpp : startup.

#include <iostream>
#include "Board.h"
#include "ChessMove.h"
#include <vector>
#include <regex>
#include <chrono>
#include <thread>

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

    //initialize the movehistory as empty. (0 entries)
    std::vector<ChessMove> moveHistory(0);

    //this defines valid input for moves.
    std::regex rgxPattern("([a-h][1-8])\\s([a-h][1-8])");

    while (true)
    {
        board->Render();

        //prompt input:
        std::cout << "Make your move: " << std::endl;
        //get input
        std::cin >> a;

        //reset
        if (a == "RESET")
        {
            board->Reset();
            player = 0;
            //dont forget to empty the history.
            moveHistory.clear();
            system("CLS");
            continue;
        }
        //replay
        if (a == "REPLAY")
        {
            board->Reset();
            int moves = 0;
            int startMove = 0;
            //second param is the amount of previous moves.
            std::cin >> moves;

            //condition for starting at the beginning of the game.
            if (moves > moveHistory.size() || moves <= 0)
                startMove = 0;
            else
                startMove = moveHistory.size() - moves;
            //fast forward through the moves until the point where the player(s) want to observe the replay.
            for (int i = 0; i < startMove; ++i)
            {
                ChessMove m = moveHistory.at(i);
                board->ForceMove(m.ax, m.ay, m.bx, m.by);
            }
            //show the interesting part.
            std::cout << "SHOWING REPLAY:" << std::endl;
            board->Render();
            for (int i = startMove; i < moveHistory.size(); ++i)
            {
                system("CLS");
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                ChessMove m = moveHistory.at(i);
                board->ForceMove(m.ax, m.ay, m.bx, m.by);
                board->Render();
            }
            std::cout << "REPLAY OVER, NEXT PLAYER: " << player << std::endl;
            //skip the rest of the loop, restart at input.
            continue;
        }

        //get "second part" of input
        std::cin >> b;

        input = a + " " + b;
        if (input.size() < 5)
        {
            system("CLS");
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

        //record the move.
        ChessMove move(ax, ay, bx, by);

        //std::cout << "TRIED TO MOVE FROM: " << ax << ", " << ay << " TO " << bx << ", " << by << std::endl;

        system("CLS");
        if (board->TryMakeMove(ax, ay, bx, by, player))
        {
            //switch between 0 and 1.
            player = 1 - player;
            //add the move to the history.
            moveHistory.push_back(move);
            continue;
        }
        else
        {
            std::cout << "INVALID MOVE" << std::endl;
        }
    }

    return 0;
}
