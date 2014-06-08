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
  static bool IsValidMove(uint8_t (&board)[8][8], Movement & movement){
    int file_movement = abs(movement.source_file - movement.dest_file);
    int rank_movement = abs(movement.source_rank - movement.dest_rank);
    assert(movement.dest_rank >= 0 && movement.dest_rank < 8);
    assert(movement.dest_file >= 0 && movement.dest_file < 8);

    if(movement.source_file == -1 || movement.source_rank == -1 ) {
      // piece not in the board
      return false;
    } else if(file_movement <= 1 && rank_movement <= 1 &&
        (file_movement == 1 || rank_movement == 1)) {
      // one square in any direction
      return true;
    }
    return false;
  }

  bool Castle(bool short_castle);
  bool HasCastle(bool short_castle) const;
};

}
}

#endif
