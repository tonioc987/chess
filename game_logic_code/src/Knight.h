/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef KNIGHT_H_
#define KNIGHT_H_

#include "Piece.h"

namespace acortes {
namespace chess {

class Knight : public Piece {
public:
  Knight(Player * player);
  bool IsValidMove(int new_file, int new_rank) const;
  std::string GetLongName() const;
  std::string GetShortName() const;
  ~Knight();
  static const std::string LongName;
};

}
}

#endif
