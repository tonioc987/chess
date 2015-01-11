/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef UCIREADER_H_
#define UCIREADER_H_

#include <string>
#include <vector>
#include "ChessReader.h"

namespace acortes {
namespace chess {

class UCIReader : ChessReader {
 public:
  UCIReader() {}
  ~UCIReader() {}
  void GetMoves(std::string moves_str, std::vector<Movement*> *moves);

 protected:
  Movement* ParseMove(std::string move);
};

}  // namespace chess
}  // namespace acortes

#endif
