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
  static bool IsValidMove(uint8_t (&board)[8][8], Movement & movement){
    int file_movement = abs(movement.source_file - movement.dest_file);
    int rank_movement = abs(movement.source_rank - movement.dest_rank);
    assert(movement.dest_rank >= 0 && movement.dest_rank < 8);
    assert(movement.dest_file >= 0 && movement.dest_file < 8);

    if(movement.source_file == -1 || movement.source_rank == -1 ) {
      // piece not in the board
      return false;
    } else if(file_movement == 0 && rank_movement == 0) {
      // no movement
      return false;
    } else if(file_movement == 0 && rank_movement !=0) {
      // forward/backward movement
      // check no pieces in between
      int start_rank = (movement.source_rank < movement.dest_rank) ? movement.source_rank + 1 : movement.dest_rank + 1;
      int end_rank = (movement.source_rank < movement.dest_rank) ? movement.dest_rank : movement.source_rank;
      for(int r = start_rank; r < end_rank; r++) {
        if(PieceType::EMPTY != board[r][movement.source_file]) {
          return false;
        }
      }
      return true;
    } else if(file_movement != 0 && rank_movement ==0) {
      // left/right movement
      // check no pieces in between
      int start_file = (movement.source_file < movement.dest_file) ? movement.source_file + 1 : movement.dest_file + 1;
      int end_file = (movement.source_file < movement.dest_file) ? movement.dest_file : movement.source_file;
      for(int f = start_file; f < end_file; f++) {
        if(PieceType::EMPTY != board[movement.source_rank][f]) {
          return false;
        }
      }
      return true;
    }
    return false;
  }
};

}
}

#endif
