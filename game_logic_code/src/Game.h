/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef GAME_H_
#define GAME_H_

#include "Common.h"
#include <vector>
#include <map>

namespace acortes {
namespace chess {

struct Movement;

typedef  bool (*IsValidFunction)(uint8_t (&board)[8][8], int file, int rank, Movement & movement);
typedef std::map<PieceType, IsValidFunction> IsValidMap;

class Game {
public:
  Game();
  void InitialSetup();
  bool Move(Movement * move);
  std::string FEN() const;
  bool IsWhiteTurn() const { return is_white_turn_;}
  std::string GetLastMove();
  void Print(char (* printed_board)[64]) const;

private:
  uint8_t board_[8][8];
  std::vector<Movement *> movements_;
  bool is_white_turn_;
  int halfmove_clock_;
  int en_passant_file_;
  int en_passant_rank_;
  int en_passant_capture_rank_;
  bool white_short_castle_;
  bool white_long_castle_;
  bool black_short_castle_;
  bool black_long_castle_;
  static const  IsValidMap is_valid_move_;
  bool FindPiece(Movement * move);
  bool FindPiece(PieceType piece, int file = -1, int rank = -1) const;
};

}
}

#endif
