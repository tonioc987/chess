/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include <cassert>
#include "Piece.h"

namespace acortes {
namespace chess {

Piece::Piece(Board * board, Color color) :
  board_(board), color_(color){
  assert(board != nullptr);
  rank_ = -1;
  file_ = -1;
  num_moves_ = 0;
}

void Piece::Put(uint8_t file, uint8_t rank) {
  file_ = file;
  rank_ = rank;
}

void Piece::Put(std::string move) {
  Put(GetFile(move), GetRank(move));
}

void Piece::Move(uint8_t file, uint8_t rank) {
  Put(file, rank);
  num_moves_++;
}

void Piece::Move(std::string move) {
  Move(GetFile(move), GetRank(move));
}

bool Piece::IsValidMove(std::string move) const {
  return IsValidMove(GetFile(move), GetRank(move));
}

Piece::~Piece() {

}

}
}
