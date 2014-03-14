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
  Bishop(Player * player);
  bool IsValidMove(int new_file, int new_rank) const;
  std::string GetLongName() const;
  std::string GetShortName() const;
  ~Bishop();
  static const std::string LongName;
};

}
}

#endif
