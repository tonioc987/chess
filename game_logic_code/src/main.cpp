#include <iostream>
#include "Game.h"
#include "Player.h"
#include "PGNPlayer.h"
#include "Board.h"
#include "PGNReader.h"

using namespace std;
using namespace acortes::chess;

int main()
{
  Board *board = new Board(8,8);
  PGNReader pgn("/media/common/prgrmmng/chess/game_logic_test/20140212.pgn");
  PGNPlayer *player1 = new PGNPlayer(Color::Light, &pgn);
  PGNPlayer *player2 = new PGNPlayer(Color::Dark, &pgn);

  Game game(board, player1, player2);
  game.InitialSetup();
  while(game.Move()) {
    cout << game.FEN() << endl;
  }

  return 0;
}
