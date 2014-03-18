/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include "Pawn.h"
#include "Board.h"
#include <cassert>

namespace acortes {
namespace chess {

const std::string Pawn::LongName = "Pawn";

Pawn::Pawn(Player * player) : Piece(player) {

}

Pawn::~Pawn() {

}

std::string Pawn::GetLongName() const {
  return Pawn::LongName;
}

std::string Pawn::GetShortName() const {
  return "P";
}

bool Pawn::IsValidMove(const int new_file, const int new_rank) const {
  const int rank_initial = (GetColor() == Color::Light) ? 1 : 6 ;
  const int rank_one_step = (GetColor() == Color::Light) ? rank_+1 : rank_-1 ;
  const int rank_two_steps = (GetColor() == Color::Light) ? rank_+2 : rank_-2 ;

  // forward move within the same file
  // when they are in the starting position, they can move two squares
  // No pieces should be in front of the pawn
  if (new_file == file_ &&
      /*&& check no piece*/
      ( (new_rank == rank_one_step) ||
        (rank_ == rank_initial && new_rank == rank_two_steps /*&& check no piece*/))) {
    return true;
  }

  // side move to capture to the left
  if (file_ > 0 &&
      new_file == file_-1 &&
      new_rank == rank_one_step/*&&
      is there an enemy piece */) {
    return true;
  }

  // side move to capture to the right
  if (file_ < 8 /*replace by board size*/-1 &&
      new_file == file_+1 &&
      new_rank == rank_one_step) {
    return true;
  }

  // TODO en passant capture
  return false;
}

void Pawn::Move(int file, int rank, bool is_capture) {
  // handle en passant, if not delegate to base class
  if(is_capture) {
    Piece * en_passant_piece = board_->GetEnPassantCandidate();
    if(en_passant_piece != nullptr) {
      int en_passant_file = en_passant_piece->GetFile();
      int en_passant_rank = en_passant_piece->GetRank();
      // if this is a white piece, the en passant target is a black piece
      // so the piece is actually located one rank less (i.e. black piece in
      // f5, target en passant f6)
      if (GetColor() == Color::Light) {
        // black en passant
        en_passant_rank++;
      } else {
        // whithe en passant
        en_passant_rank--;
      }

      if(file == en_passant_file && rank == en_passant_rank) {
        // put the en passant target in the target square and
        // delegate the rest of the processing to the base class
        board_->RemovePiece(en_passant_piece->GetFile(), en_passant_piece->GetRank());
        en_passant_piece->Put(board_, file, rank);
      }
    }
  }
  Piece::Move(file, rank, is_capture);
}

}
}
