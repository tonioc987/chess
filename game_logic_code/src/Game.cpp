/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <cassert>
#include "Game.h"
#include "Board.h"
#include "PGNReader.h"

using namespace std;

namespace acortes {
namespace chess {


Game::Game() {
  initial_board_ = new Board;
}

Game::Game(PGNReader & pgn) {
  int current_move = 0;
  Movement * move = nullptr;
  Board * current_board = nullptr;

  initial_board_ = new Board;
  current_board = initial_board_;

  while((move = pgn.GetMove(current_move))) {
    // clone existing board, note also next, previous and
    // alternative pointer are shallowed copied
    Board * new_board = new Board(*current_board);
    new_board->Move(move);
    current_board->next = new_board;
    new_board->previous = current_board;
    new_board->next = nullptr;
    new_board->alternative = nullptr;
    current_board = current_board->next;
    current_move++;
  }
}


Board * Game::InitialBoard() {
  return initial_board_->next;
}

}
}
