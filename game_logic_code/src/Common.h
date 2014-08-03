/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef COMMON_H_
#define COMMON_H_

#include <cstdint>
#include <string>

namespace acortes {
namespace chess {

#define KING 'K'
#define QUEEN 'Q'
#define BISHOP 'B'
#define KNIGHT 'N'
#define ROOK 'R'
#define PAWN 'P'
#define EMPTY '.'


bool inline AreSimilarPieces(char piece1, char piece2) { return (toupper(piece1) == toupper(piece2)); }
bool inline IsEmpty(char piece) { return ('.' == piece); }

char inline White(char piece) { return (toupper(piece)); }
char inline Black(char piece) { return (tolower(piece)); }
bool inline IsWhite(char piece) { return (isupper(piece)); }
bool inline IsBlack(char piece) { return (islower(piece)); }

struct Movement {
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
  char promoted_piece;
  std::string move;
  char piece;

  Movement() {
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
    promoted_piece = EMPTY;
    move = "";
    piece = EMPTY;
  }
};


inline int GetFile(char file) {
  return static_cast<int>(file -'a');
}

inline int GetRank(char rank) {
  return static_cast<int>(rank-'1');
}

inline char GetFile(int file) {
  return static_cast<char>(file + 'a');
}

inline char GetRank(int rank) {
  return static_cast<char>(rank + '1');
}

}  // namespace chess
}  // namespace acortes

#endif
