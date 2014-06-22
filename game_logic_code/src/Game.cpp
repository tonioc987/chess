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

const IsValidMap Game::is_valid_move_ = {
    { PieceType::KING, King::IsValidMove },
    { PieceType::QUEEN, Queen::IsValidMove },
    { PieceType::BISHOP, Bishop::IsValidMove },
    { PieceType::KNIGHT, Knight::IsValidMove },
    { PieceType::ROOK, Rook::IsValidMove },
    { PieceType::PAWN, Pawn::IsValidMove }
};

Game::Game() :
  is_white_turn_(true),
  halfmove_clock_(0),
  en_passant_file_{-1},
  en_passant_rank_{-1},
  en_passant_capture_rank_{-1} {
}


void Game::InitialSetup() {
  board_[0][0] = Color::WHITE | PieceType::ROOK;
  board_[0][1] = Color::WHITE | PieceType::KNIGHT;
  board_[0][2] = Color::WHITE | PieceType::BISHOP;
  board_[0][3] = Color::WHITE | PieceType::QUEEN;
  board_[0][4] = Color::WHITE | PieceType::KING;
  board_[0][5] = Color::WHITE | PieceType::BISHOP;
  board_[0][6] = Color::WHITE | PieceType::KNIGHT;
  board_[0][7] = Color::WHITE | PieceType::ROOK;

  for(int i = 0; i < 8; ++i) {
    board_[1][i] = Color::WHITE | PieceType::PAWN;
  }

  for(int k = 2; k < 6; ++k) {
    for(int i = 0; i < 8; ++i) {
      board_[k][i] = static_cast<uint8_t>(PieceType::EMPTY);
    }
  }

  for(int i = 0; i < 8; ++i) {
    board_[6][i] = Color::BLACK | PieceType::PAWN;
  }

  board_[7][0] = Color::BLACK | PieceType::ROOK;
  board_[7][1] = Color::BLACK | PieceType::KNIGHT;
  board_[7][2] = Color::BLACK | PieceType::BISHOP;
  board_[7][3] = Color::BLACK | PieceType::QUEEN;
  board_[7][4] = Color::BLACK | PieceType::KING;
  board_[7][5] = Color::BLACK | PieceType::BISHOP;
  board_[7][6] = Color::BLACK | PieceType::KNIGHT;
  board_[7][7] = Color::BLACK | PieceType::ROOK;

  white_short_castle_ = true;
  white_long_castle_ = true;
  black_short_castle_ = true;
  black_long_castle_ = true;
}


bool Game::Move(Movement * move) {
  assert(move != nullptr);

  if(FindPiece(move)){

    if(move->is_short_castle) {
      // move king
      board_[move->source_rank][move->source_file+2] =
          board_[move->source_rank][move->source_file];
      board_[move->source_rank][move->source_file] =
          static_cast<uint8_t>(PieceType::EMPTY);
      // move rook
      board_[move->source_rank][move->source_file+2-1] =
          board_[move->source_rank][move->source_file+3];
      board_[move->source_rank][move->source_file+3] =
          static_cast<uint8_t>(PieceType::EMPTY);
    } else if(move->is_long_castle) {
      board_[move->source_rank][move->source_file-2] =
          board_[move->source_rank][move->source_file];
      board_[move->source_rank][move->source_file] =
          static_cast<uint8_t>(PieceType::EMPTY);
      // move rook
      board_[move->source_rank][move->source_file-2+1] =
          board_[move->source_rank][move->source_file-4];
      board_[move->source_rank][move->source_file-4] =
          static_cast<uint8_t>(PieceType::EMPTY);
    } else {
      if(move->is_capture) {
        // TODO: validate there is a piece of the other color
        if((PieceType::PAWN == move->piece) &&
            (en_passant_file_ == move->dest_file) &&
            (en_passant_capture_rank_ == move->dest_rank)) {
          board_[en_passant_rank_][en_passant_file_] =
              static_cast<uint8_t>(PieceType::EMPTY);
        }
      }
      board_[move->dest_rank][move->dest_file] = board_[move->source_rank][move->source_file];
      board_[move->source_rank][move->source_file] = static_cast<uint8_t>(PieceType::EMPTY);
    }
  } else {
    assert(false);
  }

  // TODO: if rook movement, remove appropriate castle
  if(PieceType::KING == move->piece) {
    if(Color::WHITE == move->color) {
      white_short_castle_ = false;
      white_long_castle_ = false;
    } else {
      black_short_castle_ = false;
      black_long_castle_ = false;
    }
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
     (abs(move->dest_rank - move->source_rank) == 2)) {
    en_passant_file_ = move->dest_file;
    en_passant_rank_ = move->dest_rank;
    en_passant_capture_rank_ = move->dest_rank -
        ((move->dest_rank - move->source_rank)/2);
  } else {
    en_passant_file_ = -1;
    en_passant_rank_ = -1;
    en_passant_capture_rank_ = -1;
  }

  return move!=nullptr;
}


bool Game::FindPiece(Movement * move) {
  bool found = false;
  /* TODO: is it really necessary to remove this assertions to make
   * it work for the castle, or is it just better to enable the
   * function below and simplify things.
   * assert(move->dest_file >= 0);
  assert(move->dest_file < 8);
  assert(move->dest_rank >= 0);
  assert(move->dest_rank < 8);*/
  IsValidMap::const_iterator it = Game::is_valid_move_.find(move->piece);
  IsValidFunction valid = *(it->second);

  for(auto rank = 7; rank != -1; --rank) {
    for(auto file = 0; file != 8; ++file) {
      // check if piece can reach the destination square
      // if can reach square, then filter by source square in case
      // the information exists
      if( move->piece == board_[rank][file] &&
          move->color == board_[rank][file] &&
          valid(board_, file, rank, *move) &&
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
  for(auto rank = 7; rank != -1; --rank) {
    for(auto file = 0; file != 8; ++file) {
      if(PieceType::EMPTY == board_[rank][file]) {
        ++empty_spaces;
      } else {
        if(empty_spaces) {
          fen.append(std::to_string(empty_spaces));
          empty_spaces = 0;
        }
        fen.append(piece_to_string(board_[rank][file]));
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
  if(en_passant_file_ != -1) {
    fen.append(1, GetFile(en_passant_file_));
    fen.append(1, GetRank(en_passant_capture_rank_));
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
      if(PieceType::EMPTY == board_[rank][file]) {
        *c = space;
      } else {
        *c = piece_to_string(board_[rank][file]).c_str()[0];
      }
      c++;
    }
  }
}

}
}
