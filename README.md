chess
=====

Simple chess program to be used when studying chess
First steps will be to create a class hierarchy for objects.

To execute it, after building it, copy the chess engine executable in the 
Debug directory (e.g. stockfish). The program reads a .config file for 
common parameters, or the parameters can be set through command line.
If just one parameter is provided, it is assumed it is the name of the pgn 
file to analyze.

Some notes for future development:

1) Reorganize directories, fix h guards (e.g. acortes/chess/... -->
   #ifndef ACORTES_CHESS_...), and run cpplint.py
2) Parallel analysis of a game
3) Analysis on demand
4) Propose moves

