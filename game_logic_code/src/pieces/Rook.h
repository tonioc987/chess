/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef ROOK_H_
#define ROOK_H_

#include <cassert>

namespace acortes {
namespace chess {

class Rook {
 public:
  static bool IsValidMove(const char (&board)[8][8],
      int file, int rank, const Movement & move) {
    int file_movement = abs(file - move.dest_file);
    int rank_movement = abs(rank - move.dest_rank);

    assert(move.dest_rank >= 0 && move.dest_rank < 8);
    assert(move.dest_file >= 0 && move.dest_file < 8);
    assert(move.piece == board[rank][file]);
    assert(file != -1);
    assert(rank != -1);

    if (file_movement == 0 && rank_movement == 0) {
      // no movement
      return false;
    } else if (file_movement == 0 && rank_movement !=0) {
      // forward/backward movement
      // check no pieces in between
      int start_rank = (rank < move.dest_rank) ? rank + 1 : move.dest_rank + 1;
      int end_rank = (rank < move.dest_rank) ? move.dest_rank : rank;
      for (int r = start_rank; r < end_rank; r++) {
        if (!IsEmpty(board[r][file])) {
          return false;
        }
      }
      return true;
    } else if (file_movement != 0 && rank_movement ==0) {
      // left/right movement
      // check no pieces in between
      int start_file = (file < move.dest_file) ? file + 1 : move.dest_file + 1;
      int end_file = (file < move.dest_file) ? move.dest_file : file;
      for (int f = start_file; f < end_file; f++) {
        if (!IsEmpty(board[rank][f])) {
          return false;
        }
      }
      return true;
    }
    return false;
  }
};

}  // namespace chess
}  // namespace acortes

#endif
