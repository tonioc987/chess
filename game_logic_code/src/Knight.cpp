/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include "Knight.h"
#include <cassert>

namespace acortes {
namespace chess {

const std::string Knight::LongName = "Knight";

Knight::Knight(Player * player) : Piece(player) {

}

Knight::~Knight() {

}

std::string Knight::GetLongName() const {
  return Knight::LongName;
}

std::string Knight::GetShortName() const {
  return "N";
}

bool Knight::IsValidMove(int new_file, int new_rank) const {
  int file_movement = abs(file_ - new_file);
  int rank_movement = abs(rank_ - new_rank);
  assert(new_rank >= 0 && new_rank < 8);
  assert(new_file >= 0 && new_file < 8);

  if(file_ == -1 || rank_ == -1 ) {
    // piece not in the board
    return false;
  } else if(file_movement == 1 && rank_movement == 2) {
    // forward/backward movement
    return true;
  } else if(file_movement == 2 && rank_movement == 1) {
    // left/right movement
    return true;
  }
  return false;
}

}
}
