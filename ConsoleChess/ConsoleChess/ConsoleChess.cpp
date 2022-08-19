// ConsoleChess.cpp : startup.

#include <iostream>
#include <vector>
#include <regex>
#include <chrono>
#include <thread>
#include <fstream> //file streams.
#include <map>
#include <functional>
#include <typeinfo>

#include "Board.h"
#include "ChessMove.h"

using namespace ConsoleChess;

#define CLEARSCREEN system("CLS");

//Moved all important variable declarations to the top of the file so they can be accessed.
//define the callback type for commands.
typedef void (*ECallback)();

//the words parsed from the input string.
std::array<std::string, 4> inWords;
//input string
std::string input;
//the amount of current command words
int wordCount;

//initialize the movehistory as empty. (0 entries)
std::vector<ChessMove> moveHistory(0);
//the player whose turn it is rn.
int player = 0;
//set up everything.
std::unique_ptr<Board> board(new Board());

//this defines valid input for moves.
std::regex moveRgxPattern("([a-h][1-8])\\s([a-h][1-8])");
//defines numbers.
std::regex numRgxPattern("^-{0,1}(\\d+)$");

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

void resetGame()
{
    board->Reset();
    moveHistory.clear();
    player = 0;
    CLEARSCREEN
}

//shows the replay of the game up until now.
void showReplay()
{
    board->Reset();
    int moves = 0;
    int startMove = 0;
    //if the first argument isnt a valid number, stop.
    if (!std::regex_match(inWords[1], numRgxPattern))
    {
        CLEARSCREEN
        std::cout << "invalid input. Expected syntax: replay [integer]";
        return;
    }
    //second param is the amount of previous moves.
    moves = std::stoi(inWords[1]);

    //condition for starting at the beginning of the game.
    if (moves > moveHistory.size() || moves <= 0)
        startMove = 0;
    else
        startMove = (int)moveHistory.size() - moves;
    //fast forward through the moves until the point where the player(s) want to observe the replay.
    int tplayer = 0;
    for (int i = 0; i < startMove; ++i)
    {
        ChessMove m = moveHistory.at(i);
        board->ForceMove(&m, tplayer);
        tplayer = 1 - tplayer;
    }
    //show the interesting part.
    CLEARSCREEN
    std::cout << "SHOWING REPLAY:" << std::endl;
    board->Render();
    for (int i = startMove; i < moveHistory.size(); ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        CLEARSCREEN
        ChessMove m = moveHistory.at(i);
        board->ForceMove(&m, tplayer);
        tplayer = 1 - tplayer;
        board->Render();
    }
    //clear one last time.
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    CLEARSCREEN
    std::cout << "REPLAY OVER, NEXT PLAYER: " << player << std::endl;
}

void quickSave()
{
    //out stream
    std::ofstream fileStream("quicksave.cgn");
    //first line is just the length of the array to expect.
    fileStream << moveHistory.size() << std::endl;
    for (int i = 0; i < moveHistory.size(); ++i)
    {
        fileStream << moveHistory.at(i).toUShort() << ' '; //all moves seperated by a space
    }
    fileStream.close();
}

//assumes a already reset game.
void quickLoad()
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
        //move the board. //has to happen BEFORE push_back because it only adds a copy
        board->ForceMove(&m, tplayer);
        //save the move
        moveHistory.push_back(m);
        tplayer = 1 - tplayer;
    }
    player = tplayer;
    fileStream.close();
}

//gets the amount of input words from a rawInput.
int getInput(std::string& rawInput, std::array<std::string, 4>& inWords)
{
    //the count of words found.
    int count = 0;
    //iterate over the entire length of the string.
    for (int i = 0; i < rawInput.size(); ++i)
    {
        char c = rawInput.at(i);
        //skip over whitespace
        if (c == ' ' || c == '\n')
            continue;
        //if the inWords array is full, stop searching.
        if (count >= 4)
            break;
        //found a letter, continue until the next whitespace or end of string.
        for (int j = i; j < rawInput.size(); ++j)
        {
            char cj = rawInput.at(j);
            //upon finding whitespace, add the substring to the word array.
            if (cj == ' ' || cj == '\n')
            {
                inWords[count] = rawInput.substr(i, j-i);
                ++count;
                i = j;
                break;
            }
            //convert characters to lower case for ease of use.
            rawInput[j] = std::tolower(cj);
            //check for end of string.
            if (j == rawInput.size() - 1)
            {
                inWords[count] = rawInput.substr(i, j+1 - i);
                ++count;
                i = j;
                break;
            }
        }
    }
    return count;
}

//exports the move history in algebraic notation form.
void exportMoves()
{
    std::ofstream fileStream("export.txt");
    for (int i = 0; i < moveHistory.size(); ++i)
    {
        fileStream << moveHistory.at(i).algbNot << ' ';
        if (i % 2 == 1)
            fileStream << '\n';
    }
    fileStream.close();
}

//all command method go here.
//technically some of these dont need to exist, and their respective method calls can be directly inserted into the commands map, but this is more consistent.
//also the compiler may or may not be smart enough to inline some of these? maybe?

void resetCommand()
{
    resetGame();
}
void replayCommand()
{
    //needs 2 words to work. replay and one argument.
    if (wordCount > 1)
        showReplay();
}
void loadCommand()
{
    resetGame();
    quickLoad();
}
void saveCommand()
{
    quickSave();
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
    exportMoves();
}

//main yep yep main
int main()
{
    board->Initialize();
   
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
        wordCount = getInput(input, inWords);

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
        int ay = intFromChar(moveInput[0]);
        int ax = intFromChar(moveInput[1]);
        //input[2] should be white space.
        int by = intFromChar(moveInput[3]);
        int bx = intFromChar(moveInput[4]);

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
