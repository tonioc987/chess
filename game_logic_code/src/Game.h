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

struct Board;

class Game {
public:
  Game();
  void Move(Movement * move);
  std::string FEN() const;
  bool IsWhiteTurn() const;
  std::string GetLastMove();
  void Print(char (* printed_board)[64]) const;

private:
  std::vector<Board *> board_states_;
};

}
}

#endif
