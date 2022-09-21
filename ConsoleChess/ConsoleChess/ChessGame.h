#pragma once

#include <string>
#include <vector>
#ifndef STD_IOSTREAM
#define STD_IOSTREAM
#include <iostream> //required for std::cout
#endif 
#include <fstream> //file streams
#include <map>

#ifndef STD_REGEX
#define STD_REGEX
#include <regex>
#endif

#include "Board.h"
#include "ChessMove.h"

namespace ConsoleChess
{
	//small macro for clearing the screen in the console.
#define CLEARSCREEN system("CLS");

	//define the callback type for commands.
	typedef void (*ECallback)();

	//yeah thats a command.
	struct Command
	{
	public:
		int requiredArguments;
		ECallback callback;

		Command() : requiredArguments(0), callback(NULL) { }
	
		Command(int argc, ECallback& cb) : requiredArguments(argc), callback(cb) { }
	};

	//converts an individual part of a coordinate (1-8, a-g) into the corresponding index (0-7)
	static int intFromChar(char c);

	//seperates a string into up to N words, each seperated by whitespace.
	template<UINT N>
	int getInput(std::string& rawInput, std::array<std::string, N>& inWords);

	//this is utility for the test project since i am unaware of why the template getInput<N> is unavailable.
	static int getInput4(std::string& rawInput, std::array<std::string, 4>& inWords) { return getInput<4>(rawInput, inWords); }

	class ChessGame //this basically contains most of the code that was previously inside ConsoleChess.cpp (well the headers for the code but yeah)
	{
	private:
		//whether the game is still being played.
		bool isPlayed = true;

		//the words parsed from the input string.
		std::array<std::string, 4> inWords;
		//the actual amount of words
		int wordCount = 0;

		//register commands in here.
		std::map<std::string, Command> commands;

		//the history of moves.
		std::vector<ChessMove> moveHistory;
		//the player whose turn it is rn.
		int player = 0;

		//the board the game is played on.
		std::unique_ptr<Board> board;

		//this defines valid input for moves.
		const std::regex moveRgxPattern = std::regex("([a-h][1-8])\\s([a-h][1-8])");
		//defines numbers.
		const std::regex numRgxPattern = std::regex("^-{0,1}(\\d+)$");

		//quicksave file name
		const std::string qsFileName = "quicksave.cgn";
		//export file name
		const std::string xFileName = "export.txt";

	public:
		ChessGame();

		//takes an unprocessed line of input and processes commands from it.
		void ProcessRawInput(std::string& input);

		//resets the game.
		void Reset();

		//shows a replay of the game. requires a second word (number) as input
		void ShowReplay();

		//quickly saves the current boardstate
		void QuickSave();

		//loads the boardstate from the quicksave if present.
		void QuickLoad();

		//exports the current game in standard algebraic notation
		void Export();

		//pass through to board.Render().
		void Render();

		//registers a command with a name, the amount of (additional) words, and the callback
		void RegisterCommand(std::string name, int requiredWords, ECallback callback);

		//removes the command from the map
		void UnregisterCommand(std::string);

		//id of the player whose turn it is
		int GetCurrentPlayer() { return player; }

		//is the game being played?
		bool IsPlaying() { return isPlayed; }

		//stop the game
		void Stop() { isPlayed = false; }
	};

}

