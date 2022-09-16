#include "ChessGame.h"

#include <thread>
#include <chrono>

//static helpers come first. 

static int ConsoleChess::intFromChar(char c)
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

template<UINT N>
static int ConsoleChess::getInput(std::string& rawInput, std::array<std::string, N>& inWords)
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
        if (count >= N)
            break;
        //found a letter, continue until the next whitespace or end of string.
        for (int j = i; j < rawInput.size(); ++j)
        {
            char cj = rawInput.at(j);
            //upon finding whitespace, add the substring to the word array.
            if (cj == ' ' || cj == '\n')
            {
                inWords[count] = rawInput.substr(i, j - i);
                ++count;
                i = j;
                break;
            }
            //convert characters to lower case for ease of use.
            rawInput[j] = std::tolower(cj);
            //check for end of string.
            if (j == rawInput.size() - 1)
            {
                inWords[count] = rawInput.substr(i, j + 1 - i);
                ++count;
                i = j;
                break;
            }
        }
    }
    return count;
}

//actual class members here

ConsoleChess::ChessGame::ChessGame()
{
    board->Initialize();
}

void ConsoleChess::ChessGame::ProcessRawInput(std::string& input)
{
}

void ConsoleChess::ChessGame::Reset()
{
    board->Reset();
    moveHistory.clear();
    player = 0;
    CLEARSCREEN
}

void ConsoleChess::ChessGame::ShowReplay()
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

void ConsoleChess::ChessGame::QuickSave()
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

void ConsoleChess::ChessGame::QuickLoad()
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

void ConsoleChess::ChessGame::Export()
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
