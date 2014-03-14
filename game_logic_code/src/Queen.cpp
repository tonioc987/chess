/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include "Queen.h"
#include <cassert>

namespace acortes {
namespace chess {

const std::string Queen::LongName = "Queen";

Queen::Queen(Player * player) : Piece (player) {

}

Queen::~Queen() {

}

std::string Queen::GetLongName() const {
  return Queen::LongName;
}

std::string Queen::GetShortName() const {
  return "Q";
}

bool Queen::IsValidMove(int new_file, int new_rank) const {
  int file_movement = abs(file_ - new_file);
  int rank_movement = abs(rank_ - new_rank);
  assert(new_rank >= 0 && new_rank < 8);
  assert(new_file >= 0 && new_file < 8);

  if(file_ == -1 || rank_ == -1 ) {
    // piece not in the board
    return false;
  } else if(file_movement == 0 && rank_movement == 0) {
    // no movement
    return false;
  } else if(file_movement == 0 && rank_movement !=0) {
    // forward/backward movement
    return true;
  } else if(file_movement != 0 && rank_movement ==0) {
    // left/right movement
    return true;
  } else if(file_movement == rank_movement) {
    // diagonal movement
    return true;
  }
  return false;
}

}
}

