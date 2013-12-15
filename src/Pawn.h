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
  Pawn(Board * board, Color color);
  bool IsValidMove(uint8_t new_file, uint8_t new_rank) const;
  std::string LongName() const;
  std::string ShortName() const;
  ~Pawn();
};

}
}

#endif
