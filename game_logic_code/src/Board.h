/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include "Common.h"

namespace acortes {
namespace chess {

class Piece;

class Board {
public:
  Board(int num_files, int num_ranks);
  void PutPiece(Piece * piece, int file, int rank);
  Piece * RemovePiece(int file, int rank);
  Piece * GetPiece(int file, int rank);
  std::string FEN() const;
  int GetNumFiles() { return num_files_; }
  int GetNumRanks() { return num_ranks_; }
  Piece * GetEnPassantCandidate() const { return en_passant_candidate_; }
  void SetEnPassantCandidate(Piece * piece) { en_passant_candidate_ = piece; }
  void Print(char (* printed_board)[64]) const;

protected:
  int num_files_;
  int num_ranks_;
  std::vector<Piece *> pieces_;
  std::vector<std::vector<Piece*> > board_;
  Piece * en_passant_candidate_;
};

}
}

#endif /* BOARD_H_ */
