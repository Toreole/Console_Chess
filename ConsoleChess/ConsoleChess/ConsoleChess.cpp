// ConsoleChess.cpp : startup.

#ifndef STD_IOSTREAM
#define STD_IOSTREAM
#include <iostream>
#endif
#ifndef STD_REGEX
#define STD_REGEX
#include <regex>
#endif

#include "ChessGame.h"

using namespace ConsoleChess;

#define CLEARSCREEN system("CLS");

//Moved all important variable declarations to the top of the file so they can be accessed.

ChessGame* game;

//all command method go here.
//technically some of these dont need to exist, and their respective method calls can be directly inserted into the commands map, but this is more consistent.
//also the compiler may or may not be smart enough to inline some of these? maybe?

void resetCommand()
{
    game->Reset();
}
void replayCommand()
{
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
        << "export: exports the game in algebraic notation, in file export.txt \n"
        << "quit: quits the application.";
}
void exportCommand()
{
    game->Export();
}
void quitCommand()
{
    game->Stop();
}

//main yep yep main
int main()
{
    game = new ChessGame();

    game->RegisterCommand("reset",  0, &resetCommand);
    game->RegisterCommand("replay", 1, &replayCommand);
    game->RegisterCommand("load",   0, &loadCommand);
    game->RegisterCommand("save",   0, &saveCommand);
    game->RegisterCommand("help",   0, &helpCommand);
    game->RegisterCommand("export", 0, &exportCommand);
    game->RegisterCommand("quit",   0, &quitCommand);

    std::string input;

    //the core loop.
    while (game->IsPlaying())
    {
        //show the game.
        game->Render();
        //prompt input:
        std::cout << "Make your move: (player: " << game->GetCurrentPlayer() << " )" << std::endl;
        //get raw input
        std::getline(std::cin, input);
        //input is processed by the game.
        game->ProcessRawInput(input);
    }

    delete game;

    return 0;
}
