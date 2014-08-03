/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef BOARD_H
#define BOARD_H

#include <map>
#include <vector>
#include "Common.h"

namespace acortes {
namespace chess {

typedef  bool (*IsValidFunction)(char (&board)[8][8], int file, int rank, Movement & movement);
typedef std::map<char, IsValidFunction> IsValidMap;

class Board {
public:
  Board();
  std::string FEN() const;
  void Move(Movement * move);
  bool IsWhiteTurn() const { return is_white_turn_; }
  std::string GetMove() const { return movement_->move; }
  Movement * GetMovement() const { return movement_; }
  void Print(char (* printed_board)[64]) const;

  static void AddMoves(Board * board, std::vector<Movement *> & moves);
  static Board * CreateFromPGN(std::string pgnfile);
  static void AddAlternative(Board * board, std::string alternative_str);

private:
  char board_[8][8];
  Movement * movement_;
  bool is_white_turn_;
  int move_number_;
  int halfmove_clock_;
  int en_passant_file_;
  int en_passant_rank_;
  int en_passant_capture_rank_;
  bool white_short_castle_;
  bool white_long_castle_;
  bool black_short_castle_;
  bool black_long_castle_;

  bool FindPiece(Movement * move);

  static const  IsValidMap is_valid_move_;

public:
  long centipawns;
  Board * next;
  Board * previous;
  Board * alternative;
  Board * original;
};

}
}

#endif
