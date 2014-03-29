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
  ChessEngineInterface engine;
  queue<string> lines;
  sleep(1);

  engine.WriteLine("uci");
  while(true) {
    engine.ReadLines(lines);
    while(lines.size()) {
      cout << ">>>" << lines.front() << endl;
      lines.pop();
    }
  }

  /*Board *board = new Board(8,8);
  PGNReader pgn("/home/acp/Downloads/parayuri_vs_rdom_2014_03_23.pgn");
  PGNPlayer *player1 = new PGNPlayer(Color::Light, &pgn);
  PGNPlayer *player2 = new PGNPlayer(Color::Dark, &pgn);

  Game game(board, player1, player2);
  game.InitialSetup();
  while(game.Move()) {
    cout << game.FEN() << endl;
  }*/

  return 0;
}
