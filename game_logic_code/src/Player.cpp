/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <cassert>
#include "Board.h"
#include "Player.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Movement.h"

namespace acortes {
namespace chess {

Player::Player(Color color) {
  color_ = color;
  // create pieces in order from a to h first row
  // and then all the pawns from a to h
  pieces_.push_back(new Rook(this));
  pieces_.push_back(new Knight(this));
  pieces_.push_back(new Bishop(this));
  pieces_.push_back(new Queen(this));
  pieces_.push_back(new King(this));
  pieces_.push_back(new Bishop(this));
  pieces_.push_back(new Knight(this));
  pieces_.push_back(new Rook(this));
  for(int i = 0; i < 8; ++i) {
    pieces_.push_back(new Pawn(this));
  }
}

void Player::InitialSetup(Board *board) {
  int rank = (color_ == Color::Light) ? 0 : 7;
  std::vector<Piece *>::iterator piece = pieces_.begin();

  for(int i = 0; i < 8; ++i) {
    (*piece)->Put(board, i, rank);
    ++piece;
  }

  rank = (color_ == Color::Light) ? 1 : 6;
  for(int i = 0; i < 8; ++i) {
    (*piece)->Put(board, i, rank);
    ++piece;
  }
}

Movement * Player::Move() {
  Movement *move = GetPartialMoveInformation();

  if(move != nullptr) {
    if(move->is_short_castle || move->is_long_castle) {
      assert(move->is_short_castle ^ move->is_long_castle);
      King * king = static_cast<King *>(FindPiece(King::LongName));
      king->Castle(move->is_short_castle);
      move->piece = king;
    } else {
      Piece * piece = FindPiece(move);
      piece->Move(move->dest_file, move->dest_rank, move->is_capture);
    }
    return move;
  }

  return nullptr;
}

bool Player::HasCastle(bool short_castle) const {
  King * king = static_cast<King *>(FindPiece(King::LongName));
  return king->HasCastle(short_castle);
}

Piece * Player::FindPiece(Movement * move) {
  Piece * target_piece = nullptr;

  assert(move->dest_file >= 0);
  assert(move->dest_file < 8);
  assert(move->dest_rank >= 0);
  assert(move->dest_rank < 8);

  for(auto const & piece : pieces_) {
    if( *(move->piece_type) == typeid(*piece)) {
      // check if piece can reach the destination square
      if(piece->IsValidMove(move->dest_file, move->dest_rank)) {
        // if can reach square, then filter by source square in case
        // the information exists
        if((move->source_file == -1 || piece->GetFile() == move->source_file) &&
            (move->source_rank == -1 || piece->GetRank() == move->source_rank)) {
          // just one piece shall satisfy all conditions
          assert(target_piece == nullptr);
          // this is the piece that needs to be moved!
          target_piece = piece;
          // fill information about source square
          move->source_file = piece->GetFile();
          move->source_rank = piece->GetRank();
          move->piece = target_piece;
          // at this point, it is possible to break the loop, but all pieces
          // will be evaluated in order to guaranteed that just one satisfies
          // all the conditions
        }
      }
    }
  }
  // Piece found
  assert(target_piece != nullptr);
  return target_piece;
}

Piece * Player::FindPiece(std::string long_name, int file, int rank) const {
  Piece * target_piece = nullptr;

  for(auto const & piece : pieces_) {
    if((piece->GetLongName().compare(long_name) == 0) &&
       (file == -1 || piece->GetFile() == file) &&
       (rank == -1 || piece->GetRank() == rank)) {
      assert(target_piece == nullptr);
      target_piece = piece;
    }
  }
  assert(target_piece != nullptr);
  return target_piece;
}

Player::~Player() {

}

}
}
