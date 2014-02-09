/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PLAYER_H_
#define PLAYER_H_

#include <cstdint>
#include <vector>
#include "Piece.h"
using namespace std;

namespace acortes {
namespace chess {

class Player {
public:

protected:
  Color color_;
  vector<Piece *> pieces_;
};

}
}

#endif
