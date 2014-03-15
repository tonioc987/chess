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
#include "Movement.h"
#include "Piece.h"

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
  Movement * move = players_[is_white_turn_ ? 0 : 1]->Move();
  movements_.push_back(move);
  is_white_turn_ = !is_white_turn_;
  return move!=nullptr;
}

string Game::FEN() const {
  // board position
  string fen = board_->FEN();

  // turn to move
  fen.append((is_white_turn_) ? " w" : " b");

  // TODO: castling availability
  fen.append(" ");
  bool any_castle = false;
  if(players_[0]->HasCastle(true)) {
    fen.append("K");
    any_castle = true;
  }
  if(players_[0]->HasCastle(false)) {
    fen.append("Q");
    any_castle = true;
  }
  if(players_[1]->HasCastle(true)) {
    fen.append("k");
    any_castle = true;
  }
  if(players_[1]->HasCastle(false)) {
    fen.append("q");
    any_castle = true;
  }
  if(!any_castle) {
    fen.append("-");
  }

  // TODO: en passant
  // is_white_turn_ indicates the player to move, a possible en passant pawn
  // belongs to the opposite player, that's the reason why the ternary operator
  // below looks backwards, but it is correct.
  Piece * en_passant_candidate = players_[is_white_turn_ ? 1 : 0]->GetEnPassantCandidate();
  fen.append(" ");
  if(en_passant_candidate != nullptr) {
    fen.append(1, GetFile(en_passant_candidate->GetFile()));
    int rank = en_passant_candidate->GetRank();
    // FEN records the position behind the pawn
    if(is_white_turn_) { // black is en passant candidate
      rank++;
    } else {  // white is en passant candidate
      rank--;
    }
    fen.append(1, GetRank(rank));
  } else {
    fen.append("-");
  }

  // halfmove clock
  fen.append(" 0");

  // fullmove number
  fen.append(" ");
  if(movements_.size() == 0) {
    fen.append("1");
  } else {
    fen.append(std::to_string((movements_.size()+1)/2));
  }

  return fen;
}

}
}
