/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef MOVE_H_
#define MOVE_H_

#include <typeinfo>

namespace acortes {
namespace chess {

class Piece;

struct Movement {
  const std::type_info * piece_type;
  int source_file;
  int source_rank;
  int dest_file;
  int dest_rank;
  bool is_capture;
  bool is_short_castle;
  bool is_long_castle;
  bool is_enpassant;
  bool is_check;
  bool is_mate;
  bool is_promotion;
  const std::type_info * promoted_piece;
  std::string move;
  Piece * piece;

  Movement() {
    piece_type = nullptr;
    source_file = -1;
    source_rank = -1;
    dest_file = -1;
    dest_rank = -1;
    is_capture = false;
    is_short_castle = false;
    is_long_castle = false;
    is_enpassant = false;
    is_check = false;
    is_mate = false;
    is_promotion = false;
    promoted_piece = nullptr;
    move = "";
    piece = nullptr;
  }
};

}
}

#endif
