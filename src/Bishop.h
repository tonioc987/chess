/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef BISHOP_H_
#define BISHOP_H_

#include "Piece.h"

namespace acortes {
namespace chess {

class Bishop : public Piece {
public:
  Bishop(Board * board, Color color);
  bool IsValidMove(uint8_t new_file, uint8_t new_rank) const;
  std::string LongName() const;
  std::string ShortName() const;
  ~Bishop();
};

}
}

#endif
