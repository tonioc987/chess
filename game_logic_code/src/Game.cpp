/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <cassert>
#include "Game.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

using namespace std;

namespace acortes {
namespace chess {

Game::Game() :
  is_white_turn_(true),
  halfmove_clock_(0),
  en_passant_candidate_{-1,-1} {

  is_valid_move_[PieceType::KING] = &(King::IsValidMove);
  is_valid_move_[PieceType::QUEEN] = &(Queen::IsValidMove);
  is_valid_move_[PieceType::BISHOP] = &(Bishop::IsValidMove);
  is_valid_move_[PieceType::KNIGHT] = &(Knight::IsValidMove);
  is_valid_move_[PieceType::ROOK] = &(Rook::IsValidMove);
  is_valid_move_[PieceType::PAWN] = &(Pawn::IsValidMove);
}


void Game::InitialSetup() {
  _board[0][0] = Color::WHITE | PieceType::ROOK;
  _board[0][1] = Color::WHITE | PieceType::KNIGHT;
  _board[0][2] = Color::WHITE | PieceType::BISHOP;
  _board[0][3] = Color::WHITE | PieceType::QUEEN;
  _board[0][4] = Color::WHITE | PieceType::KING;
  _board[0][5] = Color::WHITE | PieceType::BISHOP;
  _board[0][6] = Color::WHITE | PieceType::KNIGHT;
  _board[0][7] = Color::WHITE | PieceType::ROOK;

  for(int i = 0; i < 8; ++i) {
    _board[1][i] = Color::WHITE | PieceType::PAWN;
  }

  for(int k = 2; k < 6; ++k) {
    for(int i = 0; i < 8; ++i) {
      _board[k][i] = static_cast<uint8_t>(PieceType::EMPTY);
    }
  }

  for(int i = 0; i < 8; ++i) {
    _board[6][0] = Color::BLACK | PieceType::PAWN;
  }

  _board[7][0] = Color::BLACK | PieceType::ROOK;
  _board[7][1] = Color::BLACK | PieceType::KNIGHT;
  _board[7][2] = Color::BLACK | PieceType::BISHOP;
  _board[7][3] = Color::BLACK | PieceType::QUEEN;
  _board[7][4] = Color::BLACK | PieceType::KING;
  _board[7][5] = Color::BLACK | PieceType::BISHOP;
  _board[7][6] = Color::BLACK | PieceType::KNIGHT;
  _board[7][7] = Color::BLACK | PieceType::ROOK;
}


bool Game::Move(Movement * move) {
  assert(move != nullptr);

  if(move->is_short_castle || move->is_long_castle) {
    assert(move->is_short_castle ^ move->is_long_castle);
    //King * king = static_cast<King *>(FindPiece(PieceType::KING));
    //king->Castle(move->is_short_castle);
    //move->piece = king;
  } else if(FindPiece(move) &&
             is_valid_move_[move->piece](_board, *move)){

    if(move->is_capture) {
      // TODO: validate there is a piece of the other color
    }

    _board[move->dest_rank][move->dest_file] = _board[move->source_rank][move->source_file];
    _board[move->source_rank][move->source_rank] = static_cast<uint8_t>(PieceType::EMPTY);
    // if rook movement, remove appropriate castle
    // if king move remove castle
    // if castle remove castle
  } else {
    assert(false);
  }

  movements_.push_back(move);
  is_white_turn_ = !is_white_turn_;
  if(move->is_capture || (PieceType::PAWN == move->piece)) {
    halfmove_clock_ = 0;
  } else {
    halfmove_clock_++;
  }

  // store en passant candidate
  if((PieceType::PAWN == move->piece) &&
     (abs(move->source_rank - move->dest_rank) == 2)) {
    en_passant_candidate_.file = move->dest_file;
    en_passant_candidate_.rank = move->dest_rank;
  } else {
    en_passant_candidate_.file = -1;
    en_passant_candidate_.rank = -1;
  }

  return move!=nullptr;
}


bool Game::FindPiece(Movement * move) {
  bool found = false;
  assert(move->dest_file >= 0);
  assert(move->dest_file < 8);
  assert(move->dest_rank >= 0);
  assert(move->dest_rank < 8);

  for(auto rank = 8; rank != 0; --rank) {
    for(auto file = 0; file != 8; ++file) {
      // check if piece can reach the destination square
      // if can reach square, then filter by source square in case
      // the information exists
      if(move->piece == _board[rank][file] /*&& is valid move*/ &&
          (move->source_file == -1 || file == move->source_file) &&
          (move->source_rank == -1 || rank == move->source_rank)) {
        // just one piece shall satisfy all conditions
        assert(!found);
        found = true;
        // this is the piece that needs to be moved!
        // fill information about source square
        move->source_file = file;
        move->source_rank = rank;
        // at this point, it is possible to break the loop, but all pieces
        // will be evaluated in order to guaranteed that just one satisfies
        // all the conditions
      }
    }
  }

  // Piece found
  assert(found);
  return found;
}


// esto solo lo ocupo para enroque, puedo poner el cuadro origen arriba y
// no usar esta function
/*bool Game::FindPiece(PieceType piece, int file, int rank) const {
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
}*/


string Game::FEN() const {
  // board position
  int empty_spaces = 0;
  std::string fen;
  for(auto rank = 8; rank != 0; --rank) {
    for(auto file = 0; file != 8; ++file) {
      if(PieceType::EMPTY == _board[rank][file]) {
        ++empty_spaces;
      } else {
        if(empty_spaces) {
          fen.append(std::to_string(empty_spaces));
          empty_spaces = 0;
        }
        fen.append(piece_to_string(_board[rank][file]));
      }
    } // end of a rank

    if(empty_spaces) {
      fen.append(std::to_string(empty_spaces));
      empty_spaces = 0;
    }
    fen.append("/");
  }
  fen.erase(fen.size()-1);

  // turn to move
  fen.append((is_white_turn_) ? " w" : " b");

  // castling availability
  fen.append(" ");
  bool any_castle = false;
  if(white_short_castle_) {
    fen.append("K");
    any_castle = true;
  }
  if(white_long_castle_) {
    fen.append("Q");
    any_castle = true;
  }
  if(black_short_castle_) {
    fen.append("k");
    any_castle = true;
  }
  if(black_long_castle_) {
    fen.append("q");
    any_castle = true;
  }
  if(!any_castle) {
    fen.append("-");
  }

  // en passant
  fen.append(" ");
  if(en_passant_candidate_.file != -1 and en_passant_candidate_.rank != -1) {
    fen.append(1, en_passant_candidate_.file);
    int rank = en_passant_candidate_.rank;
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
  fen.append(" ");
  fen.append(std::to_string(halfmove_clock_));

  // fullmove number
  fen.append(" ");
  fen.append(std::to_string(movements_.size()/2 + 1));

  return fen;
}

string Game::GetLastMove() {
  return movements_.back()->move;
}

void Game::Print(char (* printed_board)[64]) const {
  char * c = *printed_board;
  char space = '.';

  for(auto rank = 8; rank != 0; --rank) {
    for(auto file = 0; file != 8; ++file) {
      if(PieceType::EMPTY == _board[rank][file]) {
        *c = space;
      } else {
        *c = piece_to_string(_board[rank][file]).c_str()[0];
      }
      c++;
    }
  }
}

}
}
