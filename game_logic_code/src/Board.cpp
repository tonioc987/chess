/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include <cassert>
#include "Board.h"
#include "Piece.h"
using namespace std;

namespace acortes {
namespace chess {

// initialize board to contain num_files_*num_ranks_ pointers
// to pieces
Board::Board(uint8_t num_files, uint8_t num_ranks) :
    num_files_(num_files), num_ranks_(num_ranks){
  board_.resize(num_files_);
  for(auto & files : board_) {
    files.resize(num_ranks_);
  }
}

// put the piece on the board
void Board::PutPiece(Piece * piece, uint8_t file, uint8_t rank) {
  assert(piece != nullptr);
  assert(file >= 0 && file < num_files_);
  assert(rank >= 0 && rank < num_ranks_);
  assert(piece->board_ == this);

  board_[file][rank] = piece;
  piece->Put(file, rank);
}

void Board::PutPiece(Piece * piece, std::string move) {
  PutPiece(piece, GetFile(move), GetRank(move));
}

void Board::RemovePiece(Piece * piece) {
  assert(piece != nullptr);
  assert(piece->board_ == this);

  Piece ** square_containing_piece = FindPiece(piece);
  *square_containing_piece = nullptr;
  // TODO: maybe we need to put the piece in another set of removed pieces
}

Piece** Board::FindPiece(Piece * const piece) {
  assert(piece != nullptr);

  for(auto & files : board_) {
    for(auto & cell : files) {
      if(piece == cell) {
        return & cell;
      }
    }
  }
  return nullptr;
}

}
}
