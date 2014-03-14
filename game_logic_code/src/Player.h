/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include "Common.h"

namespace acortes {
namespace chess {

class Piece;
class Board;
struct Movement;

class Player {
public:
  Player(Color color);
  void InitialSetup(Board *board);
  Color GetColor() { return color_; }
  bool Move();
  virtual ~Player();

protected:
  Color color_;
  std::vector<Piece *> pieces_;

private:
  virtual Movement * GetPartialMoveInformation() = 0;
  Piece * FindPiece(Movement * move);
  Piece * FindPiece(std::string long_name, int file = -1, int rank = -1);
};

}
}

#endif
