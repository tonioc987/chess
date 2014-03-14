/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include <cassert>
#include <algorithm>
#include <string>
#include "Piece.h"
#include "Player.h"
#include "Board.h"

namespace acortes {
namespace chess {

Piece::Piece(Player * player) :
  player_(player), board_(nullptr) {
  assert(player != nullptr);
  file_ = -1;
  rank_ = -1;
  num_moves_ = 0;
}

void Piece::Put(Board * board, int file, int rank) {
  board_ = board;
  file_ = file;
  rank_ = rank;
  board_->PutPiece(this, file, rank);
}

void Piece::Move(int file, int rank, bool is_capture) {
  if(is_capture) {
    Piece * captured_piece = board_->RemovePiece(file, rank);
    assert((GetColor() == Color::Light) ?
        (captured_piece->GetColor() == Color::Dark) :
        (captured_piece->GetColor() == Color::Light) );
    captured_piece->Captured();
  }
  Piece * this_piece = board_->RemovePiece(file_, rank_);
  assert(this == this_piece);
  Put(board_, file, rank);
  num_moves_++;
}

void Piece::Captured() {
  board_ = nullptr;
  file_ = -1;
  rank_ = -1;
}

std::string Piece::FEN() const {
  std:: string name = GetShortName();
  if(player_->GetColor() == Color::Light) {
    name[0] = toupper(name[0]);
  } else {
    name[0] = tolower(name[0]);
  }
  return name;
}

Color Piece::GetColor() const {
  return player_->GetColor();
}

Piece::~Piece() {

}

}
}
