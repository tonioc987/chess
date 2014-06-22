/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef KNIGHT_H_
#define KNIGHT_H_

#include <cassert>

namespace acortes {
namespace chess {

class Knight {
public:
  static bool IsValidMove(uint8_t (&board)[8][8], int file, int rank, Movement & move){
    int file_movement = abs(file - move.dest_file);
    int rank_movement = abs(rank - move.dest_rank);

    assert(move.dest_rank >= 0 && move.dest_rank < 8);
    assert(move.dest_file >= 0 && move.dest_file < 8);
    assert(move.piece == board[rank][file]);
    assert(move.color == board[rank][file]);
    assert(file != -1);
    assert(rank != -1);

    if(file_movement == 1 && rank_movement == 2) {
      // forward/backward movement
      return true;
    } else if(file_movement == 2 && rank_movement == 1) {
      // left/right movement
      return true;
    }
    return false;
  }
};

}
}

#endif
