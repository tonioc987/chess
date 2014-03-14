/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include "King.h"
#include "Board.h"
#include <cassert>

namespace acortes {
namespace chess {

const std::string King::LongName = "King";

King::King(Player * player) : Piece(player) {

}

King::~King() {

}

std::string King::GetLongName() const {
  return King::LongName;
}

std::string King::GetShortName() const {
  return "K";
}

bool King::IsValidMove(const int new_file, const int new_rank) const {
  int file_movement = abs(file_ - new_file);
  int rank_movement = abs(rank_ - new_rank);
  assert(new_rank >= 0 && new_rank < 8);
  assert(new_file >= 0 && new_file < 8);

  if(file_ == -1 || rank_ == -1 ) {
    // piece not in the board
    return false;
  } else if(file_movement <= 1 && rank_movement <= 1 &&
      (file_movement == 1 || rank_movement == 1)) {
    // one square in any direction
    return true;
  }
  return false;
}

bool King::Castle(bool short_castle) {
  int file_rook_start = (short_castle) ? board_->GetNumFiles() - 1 : 0 ;
  int file_rook_end = (short_castle) ? board_->GetNumFiles() - 1 - 2 : 3 ;
  int file_king_end = (short_castle) ? file_ + 2 : file_ - 2 ;
  Piece * rook = board_->GetPiece(file_rook_start, rank_);
  assert(num_moves_ == 0);
  assert(rook->GetNumMoves() == 0);
  rook->Move(file_rook_end, rank_, false);
  Move(file_king_end, rank_, false);
  return true;
}

bool King::HasCastle(bool short_castle) const {
  int file_rook_start = (short_castle) ? board_->GetNumFiles() - 1 : 0 ;
  Piece * rook = board_->GetPiece(file_rook_start, rank_);
  return (num_moves_ == 0) && (rook != nullptr) && (rook->GetNumMoves() == 0);
}


}
}
