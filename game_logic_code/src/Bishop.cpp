/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include "Bishop.h"
#include <cassert>

namespace acortes {
namespace chess {

const std::string Bishop::LongName = "Bishop";

Bishop::Bishop(Player * player) : Piece(player) {

}

Bishop::~Bishop(){

}

std::string Bishop::GetLongName() const {
  return Bishop::LongName;
}

std::string Bishop::GetShortName() const {
  return "B";
}

bool Bishop::IsValidMove(int new_file, int new_rank) const {
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
  } else if(file_movement == rank_movement) {
    // diagonal movement
    return true;
  }
  return false;
}

}
}
