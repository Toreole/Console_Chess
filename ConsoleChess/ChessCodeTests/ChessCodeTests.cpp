#include "pch.h"
#include "CppUnitTest.h"
#include "../ConsoleChess/Board.h"

#ifndef STD_REGEX
#define STD_REGEX
#include <regex>
#endif


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ChessCodeTests
{
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
	};
}
