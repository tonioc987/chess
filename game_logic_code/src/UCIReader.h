/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef UCIREADER_H_
#define UCIREADER_H_

#include "Common.h"
#include <vector>

namespace acortes {
namespace chess {

class Piece;
struct Movement;

class UCIReader {
public:
  UCIReader(std::string moves);
  ~UCIReader();
  Movement* GetMove(unsigned int n) const;

private:
  std::vector<Movement *> moves_;
  Movement* ParseMove(std::string move);
};

}
}

#endif
