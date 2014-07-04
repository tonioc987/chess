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
class PGNReader;

class Game {
public:
  Game(std::string filename);
  Board * InitialBoard();

private:
  Board * initial_board_;
};

}
}

#endif
