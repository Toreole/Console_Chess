// ConsoleChess.cpp : startup.

#include <iostream>
#include <vector>
#ifndef STD_REGEX
#define STD_REGEX
#include <regex>
#endif
#include <chrono>
#include <thread>
#include <fstream> //file streams.
#include <map>
#include <functional>

#include "Board.h"
#include "ChessMove.h"
#include "ChessGame.h"

using namespace ConsoleChess;

#define CLEARSCREEN system("CLS");

//Moved all important variable declarations to the top of the file so they can be accessed.
//define the callback type for commands.
typedef void (*ECallback)();

ChessGame* game;

//the amount of current command words
int wordCount;

//all command method go here.
//technically some of these dont need to exist, and their respective method calls can be directly inserted into the commands map, but this is more consistent.
//also the compiler may or may not be smart enough to inline some of these? maybe?

void resetCommand()
{
    game->Reset();
}
void replayCommand()
{
    //needs 2 words to work. replay and one argument.
    if (wordCount > 1)
        game->ShowReplay();
}
void loadCommand()
{
    game->Reset();
    game->QuickLoad();
}
void saveCommand()
{
    game->QuickSave();
}
void helpCommand()
{
    std::cout << "help: shows this text.\n"
        << "reset: resets the game to the very start.\n"
        << "replay [n]: shows a replay from [n] moves ago. n<=0 will show the entire game.\n"
        << "save: creates a quicksave.cgn with all moves in more or less human readable form.\n"
        << "load: loads the gamestate from the quicksave.\n"
        << "export: exports the game in algebraic notation, in file export.txt \n";
}
void exportCommand()
{
    game->Export();
}

//main yep yep main
int main()
{
    game = new ChessGame();

    //register commands in here.
    std::map<std::string, ECallback> commands;
    commands["reset"] = &resetCommand;
    commands["replay"] = &replayCommand;
    commands["load"] = &loadCommand;
    commands["save"] = &saveCommand;
    commands["help"] = &helpCommand;
    commands["export"] = &exportCommand;

    //the core loop.
    while (true)
    {
        //always start with rendering the board.
        board->Render();

        //prompt input:
        std::cout << "Make your move: (player: " << player << " )" << std::endl;

        //get raw input
        std::getline(std::cin, input);
        //get the wordCount of the input, and seperate the words out into the inWords array.
        wordCount = ConsoleChess::getInput<4>(input, inWords);

        //apparently std::map.contains(key) is a thing in c++ 20, but it looks like thats not what im on rn.
        //check if the first word is a registered command.
        if (commands.count(inWords[0]))
        {
            //run the command then continue.
            commands[inWords[0]]();
            continue;
        }

        //if the entered line was not a command, check if it abides by the move pattern.
        std::string moveInput = inWords[0] + " " + inWords[1];
        if (moveInput.size() < 5)
        {
            std::cout << "INPUT TOO SHORT, WAS SIZE: " << moveInput.size() << std::endl;
            continue;
        }
        //check it with the defined regex pattern.
        bool validInput = std::regex_match(moveInput, moveRgxPattern);
        if (!validInput)
        {
            std::cout << "move input was not valid string. expected: [field] [field] (e.g.: [d2 d4])\n";
            continue;
        }

        //this is a little scuffed and mixed up because i did something silly with the setup of the board, so it has to be like this.
        int ay = ConsoleChess::intFromChar(moveInput[0]);
        int ax = ConsoleChess::intFromChar(moveInput[1]);
        //input[2] should be white space.
        int by = ConsoleChess::intFromChar(moveInput[3]);
        int bx = ConsoleChess::intFromChar(moveInput[4]);

        //record the move.
        ChessMove move(ax, ay, bx, by);

        //clear the screen
        CLEARSCREEN
        //try to make the move.
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
