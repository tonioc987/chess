/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include "King.h"

namespace acortes {
namespace chess {

King::King(Board * board, Color color) : Piece(board, color) {

}

King::~King() {

}

std::string King::LongName() const {
  return "King";
}

std::string King::ShortName() const {
  return "K";
}

bool King::IsValidMove(const uint8_t new_file, const uint8_t) const {
  return false;
}

}
}
