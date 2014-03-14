/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <cassert>
#include "Game.h"
#include "Board.h"
#include "Player.h"

using namespace std;

namespace acortes {
namespace chess {

Game::Game(Board * board, Player * player1, Player * player2) :
  board_(board), players_{player1, player2},
  is_white_turn_(true) {
}

void Game::InitialSetup() {
  players_[0]->InitialSetup(board_);
  players_[1]->InitialSetup(board_);
}

bool Game::Move() {
  bool result = players_[is_white_turn_ ? 0 : 1]->Move();
  is_white_turn_ = !is_white_turn_;
  return result;
}

string Game::FEN() const {
  // board position
  string fen = board_->FEN();

  // turn to move
  fen.append((is_white_turn_) ? " w" : " b");

  // TODO: castling availability
  fen.append(" -");

  // TODO: en passant
  fen.append(" -");

  // halfmove clock
  fen.append(" 0");

  // fullmove number
  fen.append(" 0");

  return fen;
}

}
}
