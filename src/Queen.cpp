/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include "Queen.h"

namespace acortes {
namespace chess {

Queen::Queen(Board * board, Color color) : Piece (board, color) {

}

Queen::~Queen() {

}

std::string Queen::LongName() const {
  return "Queen";
}

std::string Queen::ShortName() const {
  return "Q";
}

bool Queen::IsValidMove(uint8_t new_file, uint8_t new_rank) const {
  return false;
}

}
}

