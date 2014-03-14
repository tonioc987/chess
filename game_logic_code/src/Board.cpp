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
Board::Board(int num_files, int num_ranks) :
    num_files_(num_files), num_ranks_(num_ranks){
  board_.resize(num_ranks_);
  for(auto & files : board_) {
    files.resize(num_files_);
  }
}

// put the piece on the board
void Board::PutPiece(Piece * piece, int file, int rank) {
  assert(piece != nullptr);
  assert(file >= 0 && file < num_files_);
  assert(rank >= 0 && rank < num_ranks_);

  board_[rank][file] = piece;
}

Piece * Board::RemovePiece(int file, int rank) {
  assert (board_[rank][file] != nullptr);

  Piece * piece = board_[rank][file];
  board_[rank][file] = nullptr;

  return piece;
}

Piece * Board::GetPiece(int file, int rank) {
  return board_[rank][file];
}

string Board::FEN() const {
  int empty_spaces = 0;
  std::string fen;
  for(auto rank = board_.rbegin(); rank != board_.rend(); ++rank) {
    for(auto cell = (*rank).begin(); cell != (*rank).end(); ++cell) {
      if(*cell == nullptr) {
        ++empty_spaces;
      } else {
        if(empty_spaces) {
          fen.append(std::to_string(empty_spaces));
          empty_spaces = 0;
        }
        fen.append((*cell)->FEN());
      }
    } // end of a rank

    if(empty_spaces) {
      fen.append(std::to_string(empty_spaces));
      empty_spaces = 0;
    }
    fen.append("/");
  }
  fen.erase(fen.size()-1);
  return fen;
}

}
}
