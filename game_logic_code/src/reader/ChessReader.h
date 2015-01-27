/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef CHESSREADER_H_
#define CHESSREADER_H_

#include <string>
#include <vector>
#include <memory>
#include "../Common.h"

namespace acortes {
namespace chess {

class ChessReader {
 public:
  ChessReader() {}
  virtual ~ChessReader() {}
  virtual void GetMoves(std::string, std::vector<std::unique_ptr<Movement>> *moves) = 0;

 protected:
  void ParseLine(std::string line, std::vector<std::unique_ptr<Movement>> *moves);
  virtual std::unique_ptr<Movement> ParseMove(std::string move) = 0;
};

}  // namespace chess
}  // namespace acortes

#endif
