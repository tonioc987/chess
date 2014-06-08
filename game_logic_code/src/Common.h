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

enum class Color : uint8_t{
  WHITE = 0x00,
  BLACK = 0x80
};

enum class PieceType : uint8_t{
  KING = 'K',
  QUEEN = 'Q',
  BISHOP = 'B',
  KNIGHT = 'N',
  ROOK = 'R',
  PAWN = 'P',
  EMPTY = 0
};

inline bool operator ==(PieceType p, uint8_t c) {
  return static_cast<uint8_t>(p) == (0x7F & c);
}

inline bool operator !=(PieceType p, uint8_t c) {
  return static_cast<uint8_t>(p) != (0x7F & c);
}

inline bool operator ==(Color c, uint8_t p) {
  return static_cast<uint8_t>(c) == (0x80 & p);
}

inline std::string piece_to_string(uint8_t p) {
  char c = static_cast<char>(p & 0x7F);
  if(p & 0x80) {
    c = tolower(c);
  }
  return std::string(1,c);
}

inline uint8_t operator |(Color c, PieceType p) {
  return (static_cast<uint8_t>(c)) + (static_cast<uint8_t>(p));
}

struct Square {
  int8_t file;
  int8_t rank;
};

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
  PieceType promoted_piece;
  std::string move;
  PieceType piece;

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
    promoted_piece = PieceType::EMPTY;
    move = "";
    piece = PieceType::EMPTY;
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
