#include <iostream>
#include "Game.h"
#include "Player.h"
#include "PGNPlayer.h"
#include "Board.h"
#include "PGNReader.h"
#include "ChessEngineInterface.h"

using namespace std;
using namespace acortes::chess;

int main()
{
  Board *board = new Board(8,8);
  PGNReader pgn("/home/acp/Downloads/pulpfiction007_vs_rdom_2014_04_10.pgn");
  PGNPlayer *player1 = new PGNPlayer(Color::Light, &pgn);
  PGNPlayer *player2 = new PGNPlayer(Color::Dark, &pgn);
  Game game(board, player1, player2);
  game.InitialSetup();

  while(game.Move()) {
    cout << game.FEN() << endl;
  }

  delete player1;
  delete player2;
  delete board;

  /*ChessEngineInterface engine(false);
  bool analize_white = false;
  bool analize_black = true;
  engine.Analyze(game, analize_white, analize_black);
   */
  return 0;
}
