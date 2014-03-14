/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef KING_H_
#define KING_H_

#include "Piece.h"

namespace acortes {
namespace chess {

class King : public Piece {
public:
  King(Player * player);
  bool IsValidMove(int new_file, int new_rank) const;
  bool Castle(bool short_castle);
  std::string GetLongName() const;
  std::string GetShortName() const;
  ~King();
  static const std::string LongName;
};

}
}

#endif
