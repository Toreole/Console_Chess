#include "Board.h"
#include <iostream>

using namespace ConsoleChess;

static int color_blue = 11;
static int color_green = 10;
static int color_dark_blue = 1;
static int color_dark_green = 2;

static int color_white = 7;

void Board::Initialize()
{
	//set up the board.
	//"white" side.
	board[0][0] = new Rook(0, 0, 0);
	board[0][1] = new Knight(0, 1, 0);
	board[0][2] = new Bishop(0, 2, 0);
	board[0][3] = new Queen(0, 3, 0);
	board[0][4] = playerA_King = new King(0, 4, 0);
	board[0][5] = new Bishop(0, 5, 0);
	board[0][6] = new Knight(0, 6, 0);
	board[0][7] = new Rook(0, 7, 0);

	for (int i = 0; i <= 7; ++i)
		board[1][i] = new Pawn(1, i, 0);

	//"black" side
	board[7][0] = new Rook(7, 0, 1);
	board[7][1] = new Knight(7, 1, 1);
	board[7][2] = new Bishop(7, 2, 1);
	board[7][3] = new Queen(7, 3, 1);
	board[7][4] = playerB_King = new King(7, 4, 1);
	board[7][5] = new Bishop(7, 5, 1);
	board[7][6] = new Knight(7, 6, 1);
	board[7][7] = new Rook(7, 7, 1);

	for (int i = 0; i <= 7; ++i)
		board[6][i] = new Pawn(6, i, 1);
}

std::string ConsoleChess::Board::intToStringCoordinates(int x, int y)
{
	const std::string letters = "abcdefgh";
	std::string str(1, letters[x]);
	str.append(std::to_string(y+1)); //coordinates are 0-7, but the numbers on the side range from 1-8.
	return str;
}

//stores ChessPiece*s that can move to x/y and are the same type as ignore in the testBuffer.
//returns the amount of found pieces.
int ConsoleChess::Board::findOtherPiecesToMoveTo(int x, int y, ChessPiece* original)
{
	int count = 0;
	//same as always, iterate the whole board for now. too lazy to make collections for each player.
	for (int xx = 0; xx <= 7; ++xx)
	{
		for (int yy = 0; yy <= 7; ++yy)
		{
			//get the piece at the position.
			ChessPiece* other = board[xx][yy];
			//check if its not null, if its not the ignored piece, if its the same colour, the same type, and if it can move to the location.
			//spread over multiple lines (kinda yucky) but better than one really really long line.
			if (other != nullptr
				&& other != original
				&& other->color == original->color
				&& other->GetCharacter() == original->GetCharacter()
				&& other->CanMoveTo(x, y, this, false)
				&& !moveCausesCheckOnSelf(x, y, other))
			{
				//if this piece can move to that tile without causing check on the players own king, add it.
				testBuffer[count] = other;
				++count;
			}
		}
	}
	return count;
}

bool ConsoleChess::Board::moveCausesCheckOnSelf(int x, int y, ChessPiece* moved)
{
	//keep a reference to the potential other piece
	ChessPiece* other = board[x][y];
	//replace it.
	board[x][y] = moved;
	//clear the moved tile.
	board[moved->row][moved->column] = nullptr;
	//get the king of the player.
	ChessPiece* king = KING_OF(moved->color);
	//check if the kings tile is attacked
	bool inCheck = IsTileAttacked(king->row, king->column, 1 - moved->color);
	//undo the temporary move made before.
	board[x][y] = other;
	board[moved->row][moved->column] = moved;
	//return the attacked value
	return inCheck;
}

void Board::Render()
{
	int color = true ? color_blue : color_green;
	
	const std::string letters = "a b c d e f g h ";

	std::cout << "------------------" << std::endl;
	
	//for (int i = 0; i <= 7; ++i)
	//	std::cout << letters[i] << ' ';
	//std::cout << std::endl;
	//instead of pushing each letter to cout individually, just do the entire string at once, with the new added spaces.
	std::cout << letters << std::endl;

	//draw all the things.
	for (int x = 0; x <= 7; ++x)
	{
		for (int y = 0; y <= 7; ++y)
		{
			//skip nulls
			if (board[x][y] == nullptr)
			{
				//use the darker colours for empty tiles because they only exist to better orientate yourself on the board.
				SetConsoleTextAttribute(console_handle, (x + y) % 2 == 1 ? color_dark_green : color_dark_blue);
				std::cout << "\u00FE "; //print a square character. -- this assumes codepage 850!
				continue;
			}
			//set the color, then draw the character.
			SetConsoleTextAttribute(console_handle, board[x][y]->color == 1 ? color_blue : color_green);
			std::cout << board[x][y]->GetCharacter() << ' ';
		}
		SetConsoleTextAttribute(console_handle, color_white);
		std::cout << (x+1) << std::endl;
	}
}

void Board::Reset()
{
	//remove all pieces from the board.
	for (int x = 0; x <= 7; ++x)
	{
		for (int y = 0; y <= 7; ++y)
		{
			ChessPiece* piece = board[x][y];
			if (piece != nullptr)
			{
				board[x][y] = nullptr;
				//temporary piece left over from promoting a pawn.
				if (piece->startRow <= -1)
				{
					delete piece;
				}
				else
				{
					//reset hasMoved and insert it into the takenPieces vector.
					piece->hasMoved = false;
					takenPieces.push_back(piece);
				}
			}
		}
	}
	//put all pieces in their original positions.
	for (int i = 0; i < takenPieces.size(); ++i)
	{
		ChessPiece* piece = takenPieces.at(i);
		board[piece->startRow][piece->startColumn] = piece;
		piece->row = piece->startRow;
		piece->column = piece->startColumn;
	}
	//clear the vector of taken pieces
	takenPieces.clear();
}

bool Board::TryMakeMove(ChessMove* move, int player)
{
	ChessPiece* piece = board[move->ax][move->ay];
	//validate that a non-empty field was selected.
	if (piece == nullptr)
	{
		std::cout << "Selected Piece was null." << std::endl;
		return false;
	}
	std::cout << "Selected Piece: " << piece->GetCharacter() << std::endl;
	//validate that this piece belongs to the player
	if (player != piece->color)
	{
		std::cout << "Selected Piece belonged to other player." << std::endl;
		return false;
	}
	//validate that the piece can move to that position
	if (!piece->CanMoveTo(move->bx, move->by, this))
	{
		std::cout << "Selected Piece cant move to target position." << std::endl;
		return false;
	}
	//the move is invalid if it causes the player to be / stay in check.
	if (moveCausesCheckOnSelf(move->bx, move->by, piece))
	{
		std::cout << "Move would cause check on yourself." << std::endl;
		return false;
	}
	//at this point, the move is guaranteed to be valid.
	
	//find out how many ambiguous moves there are for notation purposes.
	int ambiguousMoveCount = findOtherPiecesToMoveTo(move->bx, move->by, piece);

	//flag hasMoved as true after the move has been validated.
	piece->hasMoved = true;

	ChessPiece* other = board[move->bx][move->by];
	//was a piece taken in this move?
	bool takingPiece = other != nullptr;

	//update the position of the piece itself //the "previous" position is move->ax/ay
	piece->row = move->bx;
	piece->column = move->by;
	//update the board to reflect these changes.
	board[move->bx][move->by] = piece;
	board[move->ax][move->ay] = nullptr;

	//check if the moved piece is a pawn and if it stepped into a base row of the board.
	if (piece->GetCharacter() == 'P' && (move->bx == 7 || move->bx == 0))
	{
		std::cout << "Promote to: (Q, R, B, N)" << std::endl;
		char target;
		//TODO: theres gotta be a better way.
		std::cin >> target;
		ChessPiece* newPiece;
		//select the correct piece
		switch (std::tolower(target))
		{
		case 'q':
			newPiece = new Queen();
			break;
		case 'r':
			newPiece = new Rook();
			break;
		case 'b':
			newPiece = new Bishop();
			break;
		case 'n':
			newPiece = new Knight();
			break;
		default:
			newPiece = new Queen();
			break;
		}
		//negative startColumn is a sign that its not a regular piece.
		newPiece->startColumn = -1;
		newPiece->startRow = -1;
		//set the position.
		newPiece->row = move->bx;
		newPiece->column = move->by;
		newPiece->color = player;
		//mark the pawn as taken, even tho its technically promoted into the new piece.
		takenPieces.push_back(piece);
		//now place it on the board.
		board[move->bx][move->by] = newPiece;
		move->promotion = std::tolower(target);
		//as a final act, set the algb notation of the promotion move.
		std::string moveName = intToStringCoordinates(move->bx, move->by);
		moveName += '=' + newPiece->GetCharacter();
		move->algbNot = moveName;
	}
	else
	{
		move->promotion = ' ';
		
		//get the pieces character already.
		char pieceC = piece->GetCharacter();

		//important: disambiguating moves. pawn promo (handled above), captures, check, checkmate, castle (only implemented in ForceMove so far)
		//bools to check the uniqueness of the X and Y coords of the piece.
		bool uniqueX = true;
		bool uniqueY = true;
		for (int i = 0; i < ambiguousMoveCount; ++i)
		{
			ChessPiece* ambp = testBuffer[i];
			//check both conditions. dunno about the if its worth using &= instead of this, since & and && should have the same result for bools.
			uniqueX = uniqueX && ambp->row != move->ax;
			uniqueY = uniqueY && ambp->column != move->ay;
		}
		//temporary string.
		std::string notation = "";

		//include the Piece "name" only when its not a Pawn.
		if (pieceC != 'P')
			notation += pieceC;
		if (!uniqueY || pieceC == 'P') //disambiguate on Y
		{
			const std::string letters = "abcdefgh";
			notation += letters[move->ax];
		}
		if (!uniqueX) //disambiguate on X
			notation += std::to_string(move->ay);
		if (takingPiece) //mark moves where a piece is taken with x.
			notation += 'x';
		//add the coords of the destination of the move.
		notation += intToStringCoordinates(move->bx, move->by);
		//add a + if the opponents king was checked.
		ChessPiece* otherKing = KING_OF(1 - piece->color);
		if (IsTileAttacked(otherKing->row, otherKing->column, piece->color))
			notation += '+';
		//assign the notation.
		move->algbNot = notation;
	}

	if (takingPiece)
	{
		if (piece->row == -1)
			delete piece; //delete temporary pieces from promoting pawns.
		else //put the piece in the taken vector.
			takenPieces.push_back(other);
	}
	return true;
}

void Board::ForceMove(ChessMove& move, int asPlayer)
{
	//get the two? involved pieces. piece is guaranteed to never be nullptr.
	ChessPiece* piece = board[move.ax][move.ay];
	ChessPiece* other = board[move.bx][move.by];
	if (other != nullptr)
	{
		takenPieces.push_back(other);
	}
	//move it.
	board[move.ax][move.ay] = nullptr;
	if (move.promotion != ' ') //if promotion isnt none:
	{
		ChessPiece* newPiece;
		//select the correct piece
		switch (move.promotion)
		{
		case 'q':
			newPiece = new Queen();
			break;
		case 'r':
			newPiece = new Rook();
			break;
		case 'b':
			newPiece = new Bishop();
			break;
		case 'n':
			newPiece = new Knight();
			break;
		default:
			newPiece = new Queen();
			break;
		}
		//negative startColumn is a sign that its not a regular piece.
		newPiece->startColumn = -1;
		newPiece->startRow = -1;
		//set the position.
		newPiece->row = move.bx;
		newPiece->column = move.by;
		newPiece->color = asPlayer;
		//mark the pawn as taken, even tho its technically promoted into the new piece.
		takenPieces.push_back(piece);
		//now place it on the board.
		board[move.bx][move.by] = newPiece;
	}
	else
	{
		//set the moved piece.
		board[move.bx][move.by] = piece;
		piece->row = move.bx;
		piece->column = move.by;
		piece->hasMoved = true;
		//check if the move was the king castling.
		if (piece->GetCharacter() == 'K' && std::abs(move.by - move.ay) == 2)
		{
			int dir = move.by > move.ay ? 1 : -1;
			//short or long castle in notation:
			move.algbNot = dir > 0 ? "O-O" : "O-O-O";
			//y coordinate of the corner.
			int cornerY = dir > 0 ? 7 : 0;
			//get the rook
			ChessPiece* r = GetPieceAt(move.ax, cornerY);
			//flag it as moved
			r->hasMoved = true;
			//move it on the board
			board[move.ax][cornerY] = nullptr;
			board[move.ax][move.ay + dir] = r;
			//update its internal position.
			r->column = move.ay + dir;
		}
	}
}

bool ConsoleChess::Board::IsTileAttacked(int x, int y, int player)
{
	for (int xx = 0; xx < 8; ++xx)
	{
		for (int yy = 0; yy < 8; ++yy)
		{
			ChessPiece* ch = board[xx][yy];
			//only check for the opponents pieces.
			if (ch != nullptr && ch->color == player)
			{
				//the first attacker returns true.
				if (ch->CanMoveTo(x, y, this, false)) //disable printing issues for the check.
				{
					return true;
				}
			}
		}
	}
	return false;
}

//this could just aswell be a method of the King itself, but shouldnt make any real difference.
bool ConsoleChess::Board::CanCastle(King* king, int side)
{
	//can never castle when the king has been moved before.
	if (king->hasMoved)
	{
		std::cout << "King has moved before." << std::endl;
		return false;
	}
	//get the rook in the corner.
	int rookY = side > 0 ? 7 : 0;
	ChessPiece* rook = board[king->row][rookY];
	//if the rook isnt there / is there and has moved before, you cant castle.
	if (rook == nullptr || rook->hasMoved)
	{
		std::cout << "Rook is null or moved." << std::endl;
		return false;
	}
	//target y position of the kings move.
	int ty = king->column + side * 2;
	//the king may not be in check (theres probably a better way to go about this)
	if (IsTileAttacked(king->row, king->color, 1 - king->color))
	{
		std::cout << "King is attacked." << std::endl;
		return false;
	}
	//there may not be any pieces between the king and the rook.
	for (int yy = king->column + side; yy != rookY; yy += side)
	{
		if (GetPieceAt(king->row, yy) != nullptr)
		{
			std::cout << "Blocked path. y = " << yy << std::endl;
			return false;
		}
	}
	//if any of the tiles the king moves on are attacked, you cannot castle.
	for (int yy = king->column + side; yy <= ty; yy += side)
	{
		if (IsTileAttacked(king->row, yy, 1 - king->color))
		{
			std::cout << "King walking through attacked tile." << std::endl;
			return false;
		}
	}
	//if all conditions are met, castling is allowed.
	return true;
}

ConsoleChess::Board::Board()
{
	//empty arrays.
	board = std::array<std::array<ChessPiece*, 8>, 8>();
	//start with a size of 0
	takenPieces = std::vector<ChessPiece*>(0);

	//get the handle for the console.
	console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
}

ConsoleChess::Board::~Board()
{
	//delete taken pieces.
	for (int i = 0; i < takenPieces.size(); ++i)
		delete takenPieces.at(i);
	//call destructor on the vector.
	takenPieces.~vector();

	//delete all pieces in play
	for (int i = 0; i <= 7; ++i)
		for (int j = 0; j <= 7; ++j)
			if (board[i][j] != nullptr)
				delete board[i][j];

	//free the handle.
	CloseHandle(console_handle);
}
