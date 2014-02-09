/*  Chess
 *  Copyright (C) 2014  Mary Lopez
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include "gtest/gtest.h"
#include "Board.h"
#include "Piece.h"
#include "Queen.h"
#include "Pawn.h"

using namespace acortes::chess;
using namespace std;

// 8 . . . . . . . .
// 7 . . . . . . . .
// 6 . . . . . p . .
// 5 . . . . . . . .
// 4 . P . Q . . . .
// 3 . . . . . . . .
// 2 . . . . . . . .
// 1 . . . . . . . .
//   A B C D E F G H

class QueenTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    board_ = new Board(8,8);
    light_queen_ = new Queen(board_, Color::Light);
    light_pawn_ = new Pawn(board_, Color::Light);
    dark_pawn_ = new Pawn(board_, Color::Dark);
    board_->PutPiece(light_queen_, "d4");
    board_->PutPiece(light_pawn_, "b4");
    board_->PutPiece(dark_pawn_, "f6");
  }

  virtual void TearDown() {
    delete board_;
    delete light_queen_;
    delete light_pawn_;
    delete dark_pawn_;
  }

  Board * board_;
  Piece * light_queen_;
  Piece * light_pawn_;
  Piece * dark_pawn_;
};

TEST_F(QueenTest, MoveToEmptySquare) {
  ASSERT_TRUE(light_queen_->IsValidMove("Qd8"));
}

TEST_F(QueenTest, CaptureEnemyPiece) {
  ASSERT_TRUE(light_queen_->IsValidMove("Qxf4"));
}

TEST_F(QueenTest, CannotJumpPiece) {
  ASSERT_FALSE(light_queen_->IsValidMove("Qa4"));
}

TEST_F(QueenTest, CannotOcupyBusySquare) {
  ASSERT_FALSE(light_queen_->IsValidMove("Qb4"));
}
