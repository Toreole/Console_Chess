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
	//get the handle for the console.
	console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

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
					takenPieces.push_back(piece);
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

	//temporarily move the pieces.
	ChessPiece* other = board[move->bx][move->by];
	//place the moved piece in the second spot.
	board[move->bx][move->by] = piece;
	//clear the spot where it was before
	board[move->ax][move->ay] = nullptr;
	//update the position of the piece itself
	piece->row = move->bx;
	piece->column = move->by;

	//get the current players king piece.
	ChessPiece* currentPlayersKing = player == 1 ? playerB_King : playerA_King;
	
	//can they move to the kings position aka, is the king now in check?
	if (IsTileAttacked(currentPlayersKing->row, currentPlayersKing->column, 1 - player)) //disable printing issues for the check.
	{
		//undo the move previously made.
		board[move->bx][move->by] = other;
		board[move->ax][move->ay] = piece;
		piece->row = move->ax;
		piece->column = move->ay;
		std::cout << "Move would cause check on yourself." << std::endl;
		return false;
	}
			
	//check if the moved piece is a pawn and if it stepped into a base row of the board.
	if (piece->GetCharacter() == 'P' && move->bx == 7 || move->bx == 0)
	{
		std::cout << "Promote to: (Q, R, B, N)" << std::endl;
		char target;
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
	}
	else
	{
		move->promotion = ' ';
	}

	if (other != nullptr)
	{
		//put the piece in the taken vector.
		if (piece->row == -1)
			delete piece; //delete temporary pieces from promoting pawns.
		else
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
}

//this could just aswell be a method of the King itself, but shouldnt make any real difference.
bool ConsoleChess::Board::canCastle(King* king, int side)
{
	//can never castle when the king has been moved before.
	if (king->hasMoved)
		return false;
	//get the rook in the corner.
	int rookY = side > 0 ? 7 : 0;
	ChessPiece* rook = board[king->row][rookY];
	//if the rook isnt there / is there and has moved before, you cant castle.
	if (rook == nullptr || rook->hasMoved)
		return false;
	//target y position of the kings move.
	int ty = king->column + side * 2;
	//the king may not be in check (theres probably a better way to go about this)
	if (IsTileAttacked(king->row, king->color, 1 - king->color))
		return false;
	//there may not be any pieces between the king and the rook.
	for (int yy = king->column + side; yy != rookY; yy += side)
		if (GetPieceAt(king->row, yy) != nullptr)
			return false;
	//if any of the tiles the king moves on are attacked, you cannot castle.
	for (int yy = king->column + side; yy <= ty; yy += side)
		if (IsTileAttacked(king->row, yy, 1 - king->color))
			return false;
	//if all conditions are met, castling is allowed.
	return true;
}