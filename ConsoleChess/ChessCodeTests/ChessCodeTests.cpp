#include "pch.h"
#include "CppUnitTest.h"
#include "../ConsoleChess/Board.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ChessCodeTests
{
	TEST_CLASS(ChessCodeTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(1 + 1, 2);
		}
	};
}
