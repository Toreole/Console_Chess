# Console Chess

## About
This is a small little project for me to get more familiar with how C++ does things differently from C# (what i'm used to).
The aim is a fully functional console-based game of chess (no AI).
My plan is to implement the simpler rules of chess first, then include castle/en passant, and then optimize the runtime performance somewhat.

## How to play
The way it is currently implemented, you have to input two positions; the start position (the piece you want to move), and the destination. (e.g. "d2 d4", which moves the pawn on d2 to d4.)
If a move is not allowed, it will print (in most cases) the exact reason as to why that move is illegal.
Castling is currently not implemented.
TBD: the standard chess notation should be supported as input/output at some later point. (e.g. "d4", "dxe5", "O-O")

### Special Commands
"RESET": resets the game to the start.

"REPLAY [integer]": Shows a replay of the game beginning from [integer] moves prior to the latest one. "REPLAY 0" or "REPLAY [negative value]" will show the entire game from the first move.

## How to contribute
You don't. I want to get through this on my own, while having the code be available to people I want to share it with, without having to go through the hassle of manually adding them to the repository first.