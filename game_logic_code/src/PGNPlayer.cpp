/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <cassert>
#include "PGNPlayer.h"
#include "PGNReader.h"
#include "Movement.h"

namespace acortes {
namespace chess {

PGNPlayer::PGNPlayer(Color color, const PGNReader * const pgn_reader) :
    Player(color),  pgn_reader_(pgn_reader) {
  if(color == Color::Light) {
    current_move_ = 0;
  } else {
    current_move_ = 1;
  }

}

PGNPlayer::~PGNPlayer() {

}

Movement * PGNPlayer::GetPartialMoveInformation() {
  Movement * move = pgn_reader_->GetMove(current_move_);

  if(move != nullptr) {
    current_move_ += 2;
  }

  return move;
}

}
}
