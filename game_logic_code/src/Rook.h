/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef ROOK_H_
#define ROOK_H_

#include "Piece.h"

namespace acortes {
namespace chess {

class Rook : public Piece {
public:
  Rook(Player * player);
  virtual bool IsValidMove(int new_file, int new_rank) const;
  std::string GetLongName() const;
  std::string GetShortName() const;
  ~Rook();
  static const std::string LongName;
};

}
}

#endif
