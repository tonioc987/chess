/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include "Pawn.h"

namespace acortes {
namespace chess {

Pawn::Pawn(Board * board, Color color) : Piece(board, color) {

}

Pawn::~Pawn() {

}

std::string Pawn::LongName() const {
  return "Pawn";
}

std::string Pawn::ShortName() const {
  return "";
}

bool Pawn::IsValidMove(const uint8_t new_file, const uint8_t new_rank) const {
  // forward move within the same file
  // when they are in the starting position, they can move two squares
  // No pieces should be in front of the pawn
  if (new_file == file_ &&
      /*&& check no piece*/
      ( (new_rank == rank_+1) ||
        (rank_ == 1 && new_rank == rank_+2 /*&& check no piece*/))) {
    return true;
  }

  // side move to capture to the left
  if (file_ > 0 &&
      new_file == file_-1 &&
      new_rank == rank_+1/*&&
      is there an enemy piece */) {
    return true;
  }

  // side move to capture to the right
  if (file_ < 8 /*replace by board size*/-1 &&
      new_file == file_+1 &&
      new_rank == rank_+1) {
    return true;
  }

  // TODO en passant capture
  return false;
}

}
}
