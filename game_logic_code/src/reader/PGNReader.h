/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PGNREADER_H_
#define PGNREADER_H_

#include <string>
#include <vector>
#include "ChessReader.h"

namespace acortes {
namespace chess {

class PGNReader : ChessReader {
 public:
  PGNReader() {}
  ~PGNReader() {}
  void GetMoves(std::string filename, std::vector<Movement*> *moves);

 protected:
  Movement* ParseMove(std::string move);
};

}  // namespace chess
}  // namespace acortes

#endif
