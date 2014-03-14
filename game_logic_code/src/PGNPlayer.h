/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PGNPLAYER_H_
#define PGNPLAYER_H_

#include "Player.h"

namespace acortes {
namespace chess {

class PGNReader;

class PGNPlayer : public Player {
public:
  PGNPlayer(Color color, const PGNReader * const pgn_reader);
  ~PGNPlayer();

private:
  const PGNReader * const pgn_reader_;
  unsigned int current_move_;
  Movement * GetPartialMoveInformation();
};

}
}

#endif
