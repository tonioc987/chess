/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef QUEEN_H_
#define QUEEN_H_

#include "Piece.h"

namespace acortes {
namespace chess {

class Queen : public Piece {
public:
  Queen(Board * board, Color color);
  bool IsValidMove(uint8_t new_file, uint8_t new_rank) const;
  std::string LongName() const;
  std::string ShortName() const;
  ~Queen();
};

}
}

#endif
