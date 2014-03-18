/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef PIECE_H_
#define PIECE_H_

#include "Common.h"

namespace acortes {
namespace chess {

class Player;
class Board;

class Piece {
public:
  Piece(Player * player);
  virtual ~Piece();
  void Put(Board * board, int file, int rank);
  virtual void Move(int file, int rank, bool is_capture);
  Color GetColor() const;
  std::string FEN() const;
  virtual bool IsValidMove(int new_file, int new_rank) const = 0;
  virtual std::string GetLongName() const = 0;
  virtual std::string GetShortName() const = 0;
  int GetFile() {return file_;}
  int GetRank() {return rank_;}
  int GetNumMoves() { return num_moves_; }

  friend class Board;
protected:
  int file_;
  int rank_;
  int num_moves_;
  Player * player_;
  Board * board_;

private:
  void Captured();
};

}
}

#endif /* PIECE_H_ */
