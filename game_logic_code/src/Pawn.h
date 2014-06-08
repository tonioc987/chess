/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PAWN_H_
#define PAWN_H_

#include <cassert>

namespace acortes {
namespace chess {

class Pawn {
public:
  static bool IsValidMove(uint8_t (&board)[8][8], int file, int rank, Movement & move){
    bool is_white = (Color::WHITE == board[rank][file]);
    const int rank_initial = (is_white) ? 1 : 6 ;
    const int rank_one_step = (is_white) ? rank+1 : rank-1 ;
    const int rank_two_steps = (is_white) ? rank+2 : rank-2 ;

    if(move.color != board[rank][file]) {
      return false;
    }
    // forward move within the same file
    // when they are in the starting position, they can move two squares
    // No pieces should be in front of the pawn
    if (move.dest_file == file &&
        PieceType::EMPTY == board[rank_one_step][file] &&
        ( (move.dest_rank == rank_one_step) ||
          (rank == rank_initial && move.dest_rank == rank_two_steps &&
              PieceType::EMPTY == board[rank_two_steps][file] ) ) ) {
      return true;
    }

    // side move to capture to the left
    if (file > 0 &&
        move.dest_file == file-1 &&
        move.dest_rank == rank_one_step/*&&
        is there an enemy piece */) {
      return true;
    }

    // side move to capture to the right
    if (file < 8 /*replace by board size*/-1 &&
        move.dest_file == file+1 &&
        move.dest_rank == rank_one_step) {
      return true;
    }

    // en passant capture: this is handle when the move is done,
    // no special validation is needed since it is assumed that
    // a valid pgn file is being read.
    return false;
  }

  /*static void Move(int file, int rank, bool is_capture){
    // handle en passant, if not delegate to base class
    if(is_capture) {
      Piece * en_passant_piece = board_->GetEnPassantCandidate();
      if(en_passant_piece != nullptr) {
        int en_passant_file = en_passant_piece->GetFile();
        int en_passant_rank = en_passant_piece->GetRank();
        // if this is a white piece, the en passant target is a black piece
        // so the piece is actually located one rank less (i.e. black piece in
        // f5, target en passant f6)
        if (GetColor() == Color::Light) {
          // black en passant
          en_passant_rank++;
        } else {
          // whithe en passant
          en_passant_rank--;
        }

        if(file == en_passant_file && rank == en_passant_rank) {
          // put the en passant target in the target square and
          // delegate the rest of the processing to the base class
          board_->RemovePiece(en_passant_piece->GetFile(), en_passant_piece->GetRank());
          en_passant_piece->Put(board_, file, rank);
        }
      }
    }
    Piece::Move(file, rank, is_capture);
  }*/
};

}
}

#endif
