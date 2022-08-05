#pragma once
struct ChessMove
{
public:
	int ax, ay, bx, by;

	ChessMove(int ax, int ay, int bx, int by)
	{
		this->ax = ax;
		this->ay = ay;
		this->bx = bx;
		this->by = by;
	}
	ChessMove()
	{
		ax = 0;
		ay = 0;
		bx = 0;
		by = 0;
	}
};

