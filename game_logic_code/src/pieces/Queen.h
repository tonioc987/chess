/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef QUEEN_H_
#define QUEEN_H_

#include <cassert>

namespace acortes {
namespace chess {

class Queen {
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

    if(file_movement == 0 && rank_movement == 0) {
      // no movement
      return false;
    } else if(file_movement == 0 && rank_movement !=0) {
      // forward/backward movement
      return true;
    } else if(file_movement != 0 && rank_movement ==0) {
      // left/right movement
      return true;
    } else if(file_movement == rank_movement) {
      // diagonal movement
      return true;
    }
    return false;
  }
};

}
}

#endif
