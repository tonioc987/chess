/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PGNREADER_H_
#define PGNREADER_H_

#include "Common.h"
#include <vector>

namespace acortes {
namespace chess {

class Piece;
struct Movement;

class PGNReader {
public:
  PGNReader(std::string filename);
  ~PGNReader();
  Movement* GetMove(unsigned int n) const;

private:
  std::vector<Movement *> moves_;
  Movement* ParseMove(std::string move, Color color);
};

}
}

#endif
