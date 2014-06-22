/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <cassert>
#include "Game.h"
#include "Board.h"

using namespace std;

namespace acortes {
namespace chess {


Game::Game() {
  board_states_.push_back(new Board());
}

void Game::Move(Movement * move) {
  assert(move != nullptr);

  // clone existing board
  board_states_.push_back(new Board(*(board_states_.back())));
  Board & board = *(board_states_.back());
  board.Move(move);
}


string Game::FEN() const {
  return board_states_.back()->FEN();
}

string Game::GetLastMove() {
  return board_states_.back()->GetMove();
}

bool Game::IsWhiteTurn() const {
  return board_states_.back()->IsWhiteTurn();
}

void Game::Print(char (* printed_board)[64]) const {
  board_states_.back()->Print(printed_board);
}

}
}
