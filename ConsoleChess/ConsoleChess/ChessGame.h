#pragma once

#include <string>
#include <vector>
#include <iostream> //required for std::cout
#include <fstream> //file streams

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

	//converts an individual part of a coordinate (1-8, a-g) into the corresponding index (0-7)
	static int intFromChar(char c);

	//seperates a string into up to N words, each seperated by whitespace.
	template<UINT N>
	int getInput(std::string& rawInput, std::array<std::string, N>& inWords);

	class ChessGame //this basically contains most of the code that was previously inside ConsoleChess.cpp (well the headers for the code but yeah)
	{
	private:
		//the words parsed from the input string.
		std::array<std::string, 4> inWords;
		//input string
		std::string input;

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

		void ProcessRawInput(std::string& input);

		void Reset();

		void ShowReplay();

		void QuickSave();

		void QuickLoad();

		void Export();

	};
}

