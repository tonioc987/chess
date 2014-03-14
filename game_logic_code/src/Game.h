/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef GAME_H_
#define GAME_H_

#include "Common.h"

namespace acortes {
namespace chess {

class Board;
class Player;

class Game {
public:
  Game(Board * board, Player * player1, Player * player2);
  void InitialSetup();
  bool Move();
  std::string FEN() const;

private:
  Board *board_;
  Player *(players_[2]);
  bool is_white_turn_;
};

}
}

#endif
