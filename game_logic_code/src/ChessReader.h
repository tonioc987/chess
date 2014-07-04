/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef CHESSREADER_H_
#define CHESSREADER_H_

#include "Common.h"
#include <vector>

namespace acortes {
namespace chess {

class ChessReader {
public:
  ChessReader() {};
  virtual ~ChessReader() {};
  virtual void GetMoves(std::string, std::vector<Movement*> & moves) = 0;

protected:
  void ParseLine(std::string line, std::vector<Movement*> & moves);
  virtual Movement* ParseMove(std::string move) = 0;
};

}
}

#endif
