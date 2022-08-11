// ConsoleChess.cpp : startup.

#include <iostream>
#include <vector>
#include <regex>
#include <chrono>
#include <thread>
#include <fstream> //file streams.

#include "Board.h"
#include "ChessMove.h"

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

void resetGame(Board* board, std::vector<ChessMove>* history, int* player)
{
    board->Reset();
    history->clear();
    *player = 0;
    system("CLS");
}

//shows the replay of the game up until now.
void showReplay(Board* board, std::vector<ChessMove>* history, int nextPlayer)
{
    board->Reset();
    int moves = 0;
    int startMove = 0;
    //second param is the amount of previous moves.
    std::cin >> moves;

    //condition for starting at the beginning of the game.
    if (moves > history->size() || moves <= 0)
        startMove = 0;
    else
        startMove = (int)history->size() - moves;
    //fast forward through the moves until the point where the player(s) want to observe the replay.
    int tplayer = 0;
    for (int i = 0; i < startMove; ++i)
    {
        ChessMove m = history->at(i);
        board->ForceMove(m, tplayer);
        tplayer = 1 - tplayer;
    }
    //show the interesting part.
    system("CLS");
    std::cout << "SHOWING REPLAY:" << std::endl;
    board->Render();
    for (int i = startMove; i < history->size(); ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        system("CLS");
        ChessMove m = history->at(i);
        board->ForceMove(m, tplayer);
        tplayer = 1 - tplayer;
        board->Render();
    }
    //clear one last time.
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    system("CLS");
    std::cout << "REPLAY OVER, NEXT PLAYER: " << nextPlayer << std::endl;
}

void quickSave(std::vector<ChessMove>& history)
{
    //out stream
    std::ofstream fileStream("quicksave.cgn");
    //first line is just the length of the array to expect.
    fileStream << history.size() << std::endl;
    for (int i = 0; i < history.size(); ++i)
    {
        fileStream << history.at(i).toUShort() << ' '; //all moves seperated by a space
    }
    fileStream.close();
}

//assumes a already reset game.
void quickLoad(std::vector<ChessMove>& history, Board* board, int* player)
{
    //in stream
    std::ifstream fileStream("quicksave.cgn");
    if (fileStream.bad()) //this should make sure that the file exists.
        return;
    //first thing in the file is the amount of moves to read.
    int length = 0;
    fileStream >> length;
    int tplayer = 0;
    for (int i = 0; i < length; ++i)
    {
        USHORT u = 0;
        fileStream >> u;
        ChessMove m(u);
        //save the move
        history.push_back(m);
        //move the board.
        board->ForceMove(m, tplayer);
        tplayer = 1 - tplayer;
    }
    *player = tplayer;
    fileStream.close();
}

int main()
{
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

    //the core loop.
    while (true)
    {
        board->Render();

        //prompt input:
        std::cout << "Make your move: " << std::endl;
        //get input
        std::cin >> a;

        //TODO: proper command input.
        //reset
        if (a == "RESET")
        {
            resetGame(board.get(), &moveHistory, &player);
            continue;
        }
        //replay
        if (a == "REPLAY")
        {
            showReplay(board.get(), &moveHistory, player);
            //skip the rest of the loop, restart at input.
            continue;
        }
        if (a == "LOAD")
        {
            resetGame(board.get(), &moveHistory, &player);
            quickLoad(moveHistory, board.get(), &player);
            continue;
        }
        if (a == "SAVE")
        {
            quickSave(moveHistory);
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

        system("CLS");
        if (board->TryMakeMove(&move, player))
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
