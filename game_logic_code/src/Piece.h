/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PIECE_H_
#define PIECE_H_

#include "Common.h"

namespace acortes {
namespace chess {

class Board;

class Piece {
public:
  Piece(Board * board, Color color);
  virtual ~Piece();
  void Put(uint8_t file, uint8_t rank);
  void Put(std::string move);
  void Move(uint8_t file, uint8_t rank);
  void Move(std::string move);
  virtual bool IsValidMove(uint8_t new_file, uint8_t new_rank) const = 0;
  bool IsValidMove(std::string move) const;
  virtual std::string LongName() const = 0;
  virtual std::string ShortName() const = 0;

  friend class Board;
protected:
  uint8_t rank_;
  uint8_t file_;
  uint8_t num_moves_;
  Board * board_;
  Color color_;
};

}
}

#endif /* PIECE_H_ */
