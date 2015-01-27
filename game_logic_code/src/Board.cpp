/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <cassert>
#include <string>
#include <vector>
#include "Board.h"
#include "pieces/Bishop.h"
#include "pieces/King.h"
#include "pieces/Knight.h"
#include "pieces/Pawn.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "reader/PGNReader.h"
#include "reader/UCIReader.h"

namespace acortes {
namespace chess {

const IsValidMap Board::is_valid_move_ = {
    { White(KING), King::IsValidMove },
    { White(QUEEN), Queen::IsValidMove },
    { White(BISHOP), Bishop::IsValidMove },
    { White(KNIGHT), Knight::IsValidMove },
    { White(ROOK), Rook::IsValidMove },
    { White(PAWN), Pawn::IsValidMove },

    { Black(KING), King::IsValidMove },
    { Black(QUEEN), Queen::IsValidMove },
    { Black(BISHOP), Bishop::IsValidMove },
    { Black(KNIGHT), Knight::IsValidMove },
    { Black(ROOK), Rook::IsValidMove },
    { Black(PAWN), Pawn::IsValidMove }
};

Board::Board() {
  board_[0][0] = White(ROOK);
  board_[0][1] = White(KNIGHT);
  board_[0][2] = White(BISHOP);
  board_[0][3] = White(QUEEN);
  board_[0][4] = White(KING);
  board_[0][5] = White(BISHOP);
  board_[0][6] = White(KNIGHT);
  board_[0][7] = White(ROOK);

  for (int i = 0; i < 8; ++i) {
    board_[1][i] = White(PAWN);
  }

  for (int k = 2; k < 6; ++k) {
    for (int i = 0; i < 8; ++i) {
      board_[k][i] = EMPTY;
    }
  }

  for (int i = 0; i < 8; ++i) {
    board_[6][i] = Black(PAWN);
  }

  board_[7][0] = Black(ROOK);
  board_[7][1] = Black(KNIGHT);
  board_[7][2] = Black(BISHOP);
  board_[7][3] = Black(QUEEN);
  board_[7][4] = Black(KING);
  board_[7][5] = Black(BISHOP);
  board_[7][6] = Black(KNIGHT);
  board_[7][7] = Black(ROOK);

  movement_.reset(new Movement);
  is_white_turn_ = true;
  move_number_ = 0;
  halfmove_clock_ = 0;
  en_passant_file_ = -1;
  en_passant_rank_ = -1;
  en_passant_capture_rank_ = -1;
  white_short_castle_ = true;
  white_long_castle_ = true;
  black_short_castle_ = true;
  black_long_castle_ = true;

  centipawns = 0;
  next = nullptr;
  previous = nullptr;
  alternative = nullptr;
  original = nullptr;
}


Board::Board(const Board &board) {
  for (int k = 0; k < 8; ++k) {
    for (int i = 0; i < 8; ++i) {
      board_[k][i] = board[k][i];
    }
  }

  movement_.reset(nullptr);
  is_white_turn_ = board.is_white_turn_;
  move_number_ = board.move_number_;
  halfmove_clock_ = board.halfmove_clock_;
  en_passant_file_ = board.en_passant_file_;
  en_passant_rank_ = board.en_passant_rank_;
  en_passant_capture_rank_ = board.en_passant_capture_rank_;
  white_short_castle_ = board.white_short_castle_;
  white_long_castle_ = board.white_long_castle_;
  black_short_castle_ = board.black_short_castle_;
  black_long_castle_ = board.black_long_castle_;

  centipawns = 0;
  next = nullptr;
  previous = nullptr;
  alternative = nullptr;
  original = nullptr;
}


void Board::Move(std::unique_ptr<Movement> move) {
  // if the movement is from UCI notation, then it has just source square
  // add other data like: piece type, color, capture, castle.
  if (move->source_file != -1 && move->source_rank != -1 &&
      IsEmpty(move->piece)) {
    move->piece = board_[move->source_rank][move->source_file];

    if (AreSimilarPieces(move->piece, KING) && move->source_file == 4) {
      if (move->dest_file == 6) {
        move->is_short_castle = true;
      } else if (move->dest_file == 2) {
        move->is_long_castle = true;
      }
    }

    if ((!IsEmpty(board_[move->dest_rank][move->dest_file])) ||
       ( (AreSimilarPieces(move->piece, PAWN)) &&
         (en_passant_file_ == move->dest_file) &&
         (en_passant_capture_rank_ == move->dest_rank) ) ) {
      move->is_capture = true;
    }

    if (!IsEmpty(move->promoted_piece)) {
      if (IsWhite(move->piece)) {
        move->promoted_piece = White(move->promoted_piece);
      } else {
        move->promoted_piece = Black(move->promoted_piece);
      }
    }
  }

  if (FindPiece(move.get())) {
    if (move->is_short_castle) {
      // move king
      board_[move->source_rank][move->source_file+2] =
          board_[move->source_rank][move->source_file];
      board_[move->source_rank][move->source_file] = EMPTY;
      // move rook
      board_[move->source_rank][move->source_file+2-1] =
          board_[move->source_rank][move->source_file+3];
      board_[move->source_rank][move->source_file+3] = EMPTY;
    } else if (move->is_long_castle) {
      board_[move->source_rank][move->source_file-2] =
          board_[move->source_rank][move->source_file];
      board_[move->source_rank][move->source_file] = EMPTY;
      // move rook
      board_[move->source_rank][move->source_file-2+1] =
          board_[move->source_rank][move->source_file-4];
      board_[move->source_rank][move->source_file-4] = EMPTY;
    } else {
      if (move->is_capture) {
        if (AreSimilarPieces(move->piece, PAWN) &&
            (en_passant_file_ == move->dest_file) &&
            (en_passant_capture_rank_ == move->dest_rank)) {
          // simulate moving the pawn one square back,
          // anyway it will be overwritten below
          board_[move->dest_rank][move->dest_file] =
              board_[en_passant_rank_][en_passant_file_];
          board_[en_passant_rank_][en_passant_file_] = EMPTY;
        }

        // validate pieces of opposite color
        assert(IsWhite(board_[move->source_rank][move->source_file]) ^
               IsWhite(board_[move->dest_rank][move->dest_file]));
      }
      board_[move->dest_rank][move->dest_file] =
          board_[move->source_rank][move->source_file];
      board_[move->source_rank][move->source_file] = EMPTY;

      if (!IsEmpty(move->promoted_piece)) {
        board_[move->dest_rank][move->dest_file] = move->promoted_piece;
      }
    }
  } else {
    assert(false);
  }

  // Update castle availability
  if (AreSimilarPieces(move->piece, KING)) {
    if (IsWhite(move->piece)) {
      white_short_castle_ = false;
      white_long_castle_ = false;
    } else {
      black_short_castle_ = false;
      black_long_castle_ = false;
    }
  } else if (AreSimilarPieces(move->piece, ROOK)) {
    // testing just the source file is a weak test, but it is enough to
    // determine that castle has been lost. Ideally, this variables should be
    // set to false just once, but there is no side effect in setting them
    // to false multiple times.
    if (move->source_file == 0) {
      if (IsWhite(move->piece)) {
        white_long_castle_ = false;
      } else {
        black_long_castle_ = false;
      }
    } else if (move->source_file == 7) {
      if (IsWhite(move->piece)) {
        white_short_castle_ = false;
      } else {
        black_short_castle_ = false;
      }
    }
  }

  is_white_turn_ = !is_white_turn_;
  if (move->is_capture || AreSimilarPieces(move->piece, PAWN)) {
    halfmove_clock_ = 0;
  } else {
    halfmove_clock_++;
  }

  // store en passant candidate
  if (AreSimilarPieces(move->piece, PAWN) &&
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

  move_number_++;
  movement_ = std::move(move);
}


bool Board::FindPiece(Movement * move) {
  bool found = false;

  assert((move->dest_file >= 0 && move->dest_file < 8) ||
         move->is_short_castle || move->is_long_castle);
  assert((move->dest_rank >= 0 && move->dest_rank < 8) ||
         move->is_short_castle || move->is_long_castle);

  IsValidMap::const_iterator it = Board::is_valid_move_.find(move->piece);
  IsValidFunction valid = *(it->second);

  for (auto rank = 7; rank != -1; --rank) {
    for (auto file = 0; file != 8; ++file) {
      // check if piece can reach the destination square
      // if can reach square, then filter by source square in case
      // the information exists
      if (move->piece == board_[rank][file] &&
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
  if (!found) {
    assert(found);
  }
  return found;
}


std::string Board::FEN() const {
  // board position
  int empty_spaces = 0;
  std::string fen;
  for (auto rank = 7; rank != -1; --rank) {
    for (auto file = 0; file != 8; ++file) {
      if (IsEmpty(board_[rank][file])) {
        ++empty_spaces;
      } else {
        if (empty_spaces) {
          fen.append(std::to_string(empty_spaces));
          empty_spaces = 0;
        }
        fen.push_back(board_[rank][file]);
      }
    }  // end of a rank

    if (empty_spaces) {
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
  if (white_short_castle_) {
    fen.append("K");
    any_castle = true;
  }
  if (white_long_castle_) {
    fen.append("Q");
    any_castle = true;
  }
  if (black_short_castle_) {
    fen.append("k");
    any_castle = true;
  }
  if (black_long_castle_) {
    fen.append("q");
    any_castle = true;
  }
  if (!any_castle) {
    fen.append("-");
  }

  // en passant
  fen.append(" ");
  if (en_passant_file_ != -1) {
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
  fen.append(std::to_string((move_number_/2)+1));

  return fen;
}


void Board::AddMoves(Board * board, std::vector<std::unique_ptr<Movement>> *moves) {
  Board * current_board = board;

  for (auto & movement : *moves) {
    // clone existing board, note also next, previous and
    // alternative pointer are shallowed copied
    std::unique_ptr<Board> new_board(new Board(*current_board));
    new_board->Move(std::move(movement));
    new_board->previous = current_board;
    current_board->next = std::move(new_board);
    current_board = current_board->next.get();
  }
}


std::unique_ptr<Board> Board::CreateFromPGN(std::string pgnfile) {
  std::vector<std::unique_ptr<Movement>> moves;
  std::unique_ptr<Board> board(new Board);
  PGNReader pgn;
  pgn.GetMoves(pgnfile, &moves);
  AddMoves(board.get(), &moves);
  return board;
}


void Board::AddAlternative(Board * board, std::string alternative_str) {
  std::vector<std::unique_ptr<Movement>> moves;
  UCIReader reader;
  reader.GetMoves(alternative_str, &moves);

  // the root of the alternative is the parent of the current board
  // the cloned parent is just used to create the alternative branch
  // when the branch is merged, this parent is no longer needed and
  // it is deleted
  Board * cloned_parent = new Board(*(board->previous));

  // Add moves to cloned parent and then merge the alternative
  // with the original parent
  // assume there is at least one movement in the alternative
  AddMoves(cloned_parent, &moves);
  std::unique_ptr<Board> alternative = std::move(cloned_parent->next);
  alternative->original = board;
  alternative->previous = board->previous;
  board->alternative = std::move(alternative);
  delete cloned_parent;
}

}  // namespace chess
}  // namespace acortes
