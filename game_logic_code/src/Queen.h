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
  Queen(Player * player);
  bool IsValidMove(int new_file, int new_rank) const;
  std::string GetLongName() const;
  std::string GetShortName() const;
  ~Queen();
  static const std::string LongName;
};

}
}

#endif
