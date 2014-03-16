/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include "Rook.h"
#include "Board.h"
#include <cassert>

namespace acortes {
namespace chess {

const std::string Rook::LongName = "Rook";

Rook::Rook(Player * player) : Piece(player) {

}

Rook::~Rook(){

}

std::string Rook::GetLongName() const {
  return Rook::LongName;
}

std::string Rook::GetShortName() const {
  return "R";
}

bool Rook::IsValidMove(int new_file, int new_rank) const {
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
    // check no pieces in between
    int start_rank = (rank_ < new_rank) ? rank_ + 1 : new_rank + 1;
    int end_rank = (rank_ < new_rank) ? new_rank : rank_;
    for(int r = start_rank; r < end_rank; r++) {
      if(board_->GetPiece(file_, r) != nullptr) {
        return false;
      }
    }
    return true;
  } else if(file_movement != 0 && rank_movement ==0) {
    // left/right movement
    // check no pieces in between
    int start_file = (file_ < new_file) ? file_ + 1 : new_file + 1;
    int end_file = (file_ < new_file) ? new_file : file_;
    for(int f = start_file; f < end_file; f++) {
      if(board_->GetPiece(f, rank_) != nullptr) {
        return false;
      }
    }
    return true;
  }
  return false;
}

}
}
