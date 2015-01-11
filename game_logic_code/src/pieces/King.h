/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef KING_H_
#define KING_H_

#include <cassert>

namespace acortes {
namespace chess {

class King {
 public:
  static bool IsValidMove(const char (&board)[8][8],
      int file, int rank, const Movement & move){
    assert(move.piece == board[rank][file]);
    assert(file != -1);
    assert(rank != -1);

    if (move.is_short_castle || move.is_long_castle) {
      return true;
    }

    int file_movement = abs(file - move.dest_file);
    int rank_movement = abs(rank - move.dest_rank);
    /*assert(move.dest_rank >= 0 && move.dest_rank < 8);
    assert(move.dest_file >= 0 && move.dest_file < 8);*/

    if (file_movement <= 1 && rank_movement <= 1 &&
        (file_movement == 1 || rank_movement == 1)) {
      // one square in any direction
      return true;
    }
    return false;
  }
};

}  // namespace chess
}  // namespace acortes

#endif
