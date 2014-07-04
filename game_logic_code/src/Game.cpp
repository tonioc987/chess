/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <cassert>
#include <vector>
#include "Game.h"
#include "Board.h"
#include "reader/PGNReader.h"

using namespace std;

namespace acortes {
namespace chess {

Game::Game(string filename) {
  Board * current_board = nullptr;
  vector<Movement *> moves;
  PGNReader pgn;
  pgn.GetMoves(filename, moves);

  initial_board_ = new Board;
  current_board = initial_board_;

  for(auto & move : moves) {
    // clone existing board, note also next, previous and
    // alternative pointer are shallowed copied
    Board * new_board = new Board(*current_board);
    new_board->Move(move);
    current_board->next = new_board;
    new_board->previous = current_board;
    new_board->next = nullptr;
    new_board->alternative = nullptr;
    current_board = current_board->next;
  }
}


Board * Game::InitialBoard() {
  return initial_board_->next;
}

}
}
