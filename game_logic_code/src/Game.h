/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef GAME_H_
#define GAME_H_

#include "Common.h"
#include <vector>

namespace acortes {
namespace chess {

class Board;
class Player;
struct Movement;

class Game {
public:
  Game(Board * board, Player * player1, Player * player2);
  void InitialSetup();
  bool Move();
  std::string FEN() const;
  bool IsWhiteTurn() const { return is_white_turn_;}
  std::string GetLastMove();
  void Print(char (* printed_board)[64]) const;

private:
  Board *board_;
  Player *(players_[2]);
  std::vector<Movement *> movements_;
  bool is_white_turn_;
  int halfmove_clock_;
};

}
}

#endif
