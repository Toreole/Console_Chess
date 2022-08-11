#pragma once
#include <string>
struct ChessMove
{
public:
	int ax, ay, bx, by;
	char promotion = ' ';
	//the string representing the move (algebraic notation)
	std::string algbNot;

	ChessMove(int ax, int ay, int bx, int by)
	{
		this->ax = ax;
		this->ay = ay;
		this->bx = bx;
		this->by = by;
		algbNot = "";
		promotion = ' ';
	}
	ChessMove(USHORT u)
	{
		ax = (u >> 0) & 0b111;
		ay = (u >> 3) & 0b111;
		bx = (u >> 6) & 0b111;
		by = (u >> 9) & 0b111;
		int p = (u >> 12) & 0b111;
		switch (p)
		{
		default:
		case 0: promotion = ' '; break; //none.
		case 1: promotion = 'q'; break;
		case 2: promotion = 'r'; break;
		case 3: promotion = 'b'; break;
		case 4: promotion = 'n'; break;
		}
	}
	ChessMove()
	{
		ax = 0;
		ay = 0;
		bx = 0;
		by = 0;
		algbNot = "";
		promotion = ' ';
	}

	//packs the two 2d coordinates (ax,ay)(bx,by) and a possible promotion into a 16 bit integer
	USHORT toUShort()
	{
		int p = 0;
		switch (promotion)
		{
		default:
		case ' ': p = 0; break; //none.
		case 'q': p = 1; break;
		case 'r': p = 2; break;
		case 'b': p = 3; break;
		case 'n': p = 4; break;
		}
		//these & 0b111 are technically not necessarily needed, but it makes it more clear that we only care about the last 3 bits.
		USHORT u = ((ax & 0b111) << 0) | ((ay & 0b111) << 3) | ((bx & 0b111) << 6) | ((by & 0b111) << 9) | ((p & 0b111) << 12);
		return u;
	}
};

