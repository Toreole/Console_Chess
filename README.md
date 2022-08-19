# Console Chess

## About
This is a small little project for me to get more familiar with how C++ does things differently from C# (what i'm used to).
The aim is a fully functional console-based game of chess (no AI).
My plan is to implement the simpler rules of chess first, then include castle/en passant, and then optimize the runtime performance somewhat.

## How to play
The way it is currently implemented, you have to input two positions; the start position (the piece you want to move), and the destination. (e.g. "d2 d4", which moves the pawn on d2 to d4.)
If a move is not allowed, it will print (in most cases) the exact reason as to why that move is illegal.
Castling is currently done by moving the King by 2 tiles (moving the piece to the tile it would castle to, so e.g. "e1 g1").
En-passant is WIP.
Pawn promotion is in. Simply move a pawn to the opposite end of the board, and then type in the letter of the piece you want to promote to. (Defaults to Queen if no suitable input was given.)


### Special Commands
(these are case-insensitive, mixed capitalization is okay)
"RESET": resets the game to the start.

"REPLAY [integer]": Shows a replay of the game beginning from [integer] moves prior to the latest one. "REPLAY 0" or "REPLAY [negative value]" will show the entire game from the first move.

"SAVE": creates a quicksave.cgn file next to the executable which contains all moves made in a compressed format (but as a human readable string).

"LOAD": loads the game from the quicksave.cgn file if possible.

"HELP": lists the commands in the console.

"EXPORT": exports the move history of this game into an export.txt file, listing the moves according to the algebraic notation standard (see wikipedia for more info).

## How to contribute
For the most part, you don't. This is mostly a challenge for myself to get familiar with C++. (Also I never actually programmed a game of chess before, so there's that.). 
Pull Requests for fixes or feedback on the code will be considered, complete features will not. The aim of this is for me to get a better understanding of C++, not to make a game of chess with lots of cool and interesting features (although I am sorta doing that too).

