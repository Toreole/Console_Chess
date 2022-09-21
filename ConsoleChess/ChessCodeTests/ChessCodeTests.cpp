#include "pch.h"
#include "CppUnitTest.h"
#include "../ConsoleChess/Board.h"
#include "../ConsoleChess/ChessGame.h"

#include <string>

#ifndef STD_REGEX
#define STD_REGEX
#include <regex>
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ChessCodeTests
{
	//these need to be here for the time being, as the ECallback type
	//can only store non-member, non-lambda functions.
	int testInt;
	void changeTestInt()
	{
		testInt = 1;
	}

	TEST_CLASS(ChessCodeTests)
	{
	public:

		TEST_METHOD(InitBoardNoArgs)
		{
			ConsoleChess::Board *board = new ConsoleChess::Board();
			board->Initialize();
			Assert::IsNotNull(board->GetPieceAt(0, 0));
			delete board;
		}

		TEST_METHOD(InitBoardShortArgs)
		{
			ConsoleChess::Board* board = new ConsoleChess::Board();
			std::string bdesc = 
				"p0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n";
			board->Initialize(bdesc);
			Assert::IsNull(board->GetPieceAt(0, 0));
			delete board;
		}

		TEST_METHOD(InitBoardLengthyArgs)
		{
			ConsoleChess::Board* board = new ConsoleChess::Board();
			std::string bdesc =
				"p0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n";
			board->Initialize(bdesc);
			Assert::IsNull(board->GetPieceAt(0, 0));
			delete board;
		}

		TEST_METHOD(InitBoardCorrectArgs)
		{
			ConsoleChess::Board* board = new ConsoleChess::Board();
			std::string bdesc =
				"p0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0";
			board->Initialize(bdesc);
			Assert::IsNotNull(board->GetPieceAt(0, 0));
			delete board;
		}

		TEST_METHOD(BoardInitArgValidateRegex)
		{
			std::regex rgx("^([pkqrbnx][01]\\s){63}[pkqrbnx][01]$"); 
			std::string bdesc =
				"p0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0\n"
				"x0 x0 x0 x0 x0 x0 x0 x0";
			bool match = std::regex_match(bdesc, rgx);
			Assert::IsTrue(match);
		}

		TEST_METHOD(MoveOppositePlayerPiece)
		{
			ConsoleChess::Board* board = new ConsoleChess::Board();
			board->Initialize(); //default board.
			ConsoleChess::ChessMove m(1, 0, 2, 0); //the pawn on a2 to a3
			Assert::IsFalse(board->TryMakeMove(&m, 1));
			delete board;
		}

		TEST_METHOD(MoveOwnPiece)
		{
			ConsoleChess::Board* board = new ConsoleChess::Board();
			board->Initialize(); //default board.
			ConsoleChess::ChessMove m(1, 0, 2, 0); //the pawn on a2 to a3 //y = 0 => a. coords are 0-7.
			Assert::IsTrue(board->TryMakeMove(&m, 0));
			delete board;
		}

		TEST_METHOD(BasicCommand)
		{
			testInt = 0;
			ConsoleChess::ChessGame* game = new ConsoleChess::ChessGame();
			
			game->RegisterCommand("test", 0, &changeTestInt);
			std::string input = "test";
			game->ProcessRawInput(input);
			Assert::AreEqual(1, testInt);

			delete game;
		}

		TEST_METHOD(UnregisterCommand)
		{
			testInt = 0;
			ConsoleChess::ChessGame* game = new ConsoleChess::ChessGame();

			game->RegisterCommand("test", 0, &changeTestInt);
			game->UnregisterCommand("test");
			std::string input = "test";
			game->ProcessRawInput(input);
			Assert::AreEqual(0, testInt);

			delete game;
		}

		TEST_METHOD(InputParseMultipleWords)
		{
			std::string input = "one two three four";
			std::array<std::string, 4> words;

			int cw = ConsoleChess::getInput4(input, words);

			Assert::AreEqual(4, cw);
			Assert::AreEqual(std::string("one"), words[0]);
			Assert::AreEqual(std::string("two"), words[1]);
			Assert::AreEqual(std::string("three"), words[2]);
			Assert::AreEqual(std::string("four"), words[3]);
		}

		TEST_METHOD(InputParseManyWords)
		{
			std::string input = "one two three four five six seven";
			std::array<std::string, 4> words;
			int cw = ConsoleChess::getInput4(input, words);

			Assert::AreEqual(4, cw);
			Assert::AreEqual(std::string("one"), words[0]);
			Assert::AreEqual(std::string("two"), words[1]);
			Assert::AreEqual(std::string("three"), words[2]);
			Assert::AreEqual(std::string("four"), words[3]);
		}

		TEST_METHOD(InputParseOneWordWithTrailingWhiteSpace)
		{
			std::string input = "one  ";
			std::array<std::string, 4> words;
			int cw = ConsoleChess::getInput4(input, words);

			Assert::AreEqual(1, cw);
			Assert::AreEqual(std::string("one"), words[0]);
		}
	};
}
