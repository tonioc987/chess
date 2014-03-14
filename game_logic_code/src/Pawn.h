/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PAWN_H_
#define PAWN_H_

#include "Piece.h"

namespace acortes {
namespace chess {

class Pawn : public Piece {
public:
  Pawn(Player * player);
  bool IsValidMove(int new_file, int new_rank) const;
  std::string GetLongName() const;
  std::string GetShortName() const;
  ~Pawn();
  static const std::string LongName;
};

}
}

#endif
