#pragma once
#include <string>
struct ChessMove
{
public:
	int ax, ay, bx, by;
	//the string representing the move (algebraic notation)
	std::string algbNot;

	ChessMove(int ax, int ay, int bx, int by)
	{
		this->ax = ax;
		this->ay = ay;
		this->bx = bx;
		this->by = by;
		algbNot = "";
	}
	ChessMove(USHORT u)
	{
		ax = (u >> 0) & 0b111;
		ay = (u >> 3) & 0b111;
		bx = (u >> 6) & 0b111;
		by = (u >> 9) & 0b111;
	}
	ChessMove()
	{
		ax = 0;
		ay = 0;
		bx = 0;
		by = 0;
		algbNot = "";
	}

	//packs the two 2d coordinates (ax,ay)(bx,by) into a 16 bit integer
	USHORT toUShort()
	{
		USHORT u = ((ax & 0b111) << 0) | ((ay & 0b111) << 3) | ((bx & 0b111) << 6) | ((by & 0b111) << 9);
		return u;
	}
};

