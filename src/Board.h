/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include "Common.h"

namespace acortes {
namespace chess {

class Piece;

class Board {
public:
  Board(uint8_t num_files, uint8_t num_ranks);
  void PutPiece(Piece * piece, uint8_t file, uint8_t rank);
  void PutPiece(Piece * piece, std::string move);

protected:
  uint8_t num_files_;
  uint8_t num_ranks_;
  std::vector<Piece *> pieces_;
  std::vector<std::vector<Piece*> > board_;
};

}
}

#endif /* BOARD_H_ */
