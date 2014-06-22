/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PAWN_H_
#define PAWN_H_

#include <cassert>

namespace acortes {
namespace chess {

class Pawn {
public:
  static bool IsValidMove(uint8_t (&board)[8][8], int file, int rank, Movement & move){
    bool is_white = (Color::WHITE == board[rank][file]);
    const int rank_initial = (is_white) ? 1 : 6 ;
    const int rank_one_step = (is_white) ? rank+1 : rank-1 ;
    const int rank_two_steps = (is_white) ? rank+2 : rank-2 ;

    assert(move.dest_rank >= 0 && move.dest_rank < 8);
    assert(move.dest_file >= 0 && move.dest_file < 8);
    assert(move.piece == board[rank][file]);
    assert(move.color == board[rank][file]);
    assert(file != -1);
    assert(rank != -1);

    // forward move within the same file
    // when they are in the starting position, they can move two squares
    // No pieces should be in front of the pawn
    if (move.dest_file == file &&
        PieceType::EMPTY == board[rank_one_step][file] &&
        ( (move.dest_rank == rank_one_step) ||
          (rank == rank_initial && move.dest_rank == rank_two_steps &&
              PieceType::EMPTY == board[rank_two_steps][file] ) ) ) {
      return true;
    }

    // side move to capture to the left
    if (file > 0 &&
        move.dest_file == file-1 &&
        move.dest_rank == rank_one_step/*&&
        is there an enemy piece */) {
      return true;
    }

    // side move to capture to the right
    if (file < 8 /*replace by board size*/-1 &&
        move.dest_file == file+1 &&
        move.dest_rank == rank_one_step) {
      return true;
    }

    // en passant capture: this is handle when the move is done,
    // no special validation is needed since it is assumed that
    // a valid pgn file is being read.
    return false;
  }
};

}
}

#endif
