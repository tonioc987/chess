/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <memory>
#include "Common.h"

namespace acortes {
namespace chess {

typedef  bool (*IsValidFunction)(const char (&board)[8][8],
    int file, int rank, const Movement &movement);
typedef std::map<char, IsValidFunction> IsValidMap;

class Board {
 public:
  Board();
  Board(const Board &board);
  std::string FEN() const;
  void Move(std::unique_ptr<Movement> move);
  bool IsWhiteTurn() const { return is_white_turn_; }
  std::string GetMove() const { return movement_->move; }
  //Movement * GetMovement() const { return movement_; }
  char* operator[](size_t idx) { return board_[idx]; }
  const char *const operator[](size_t idx) const { return board_[idx]; }
  int move_number() const { return move_number_; }

  static void AddMoves(Board * board, std::vector<std::unique_ptr<Movement>> *moves);
  static std::unique_ptr<Board> CreateFromPGN(std::string pgnfile);
  static void AddAlternative(Board * board, std::string alternative_str);

 private:
  char board_[8][8];
  std::unique_ptr<Movement> movement_;
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
  int centipawns;
  std::unique_ptr<Board> next;
  Board * previous;
  std::unique_ptr<Board> alternative;
  Board * original;
};

}  // namespace chess
}  // namespace acortes

#endif
