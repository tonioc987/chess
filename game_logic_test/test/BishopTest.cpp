/*  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include "gtest/gtest.h"
#include "Board.h"
#include "Piece.h"
#include "Bishop.h"
#include "Pawn.h"
using namespace acortes::chess;
using namespace std;

// 8 . . . . . . . .
// 7 . . . . . . . .
// 6 . . . . . . . .
// 5 . . . . . . . .
// 4 . . . p . . . .
// 3 . . B . . . . .
// 2 . . . P . . . .
// 1 . . . . . . . .
//   A B C D E F G H
class BishopTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    board_ = new Board(8,8);
    light_bishop_ = new Bishop(board_, Color::Light);
    light_pawn_ = new Pawn(board_, Color::Light);
    dark_pawn_ = new Pawn(board_, Color::Dark);
    board_->PutPiece(light_bishop_, "c3");
    board_->PutPiece(light_pawn_, "d2");
    board_->PutPiece(dark_pawn_, "d4");
  }

  virtual void TearDown() {
    delete board_;
    delete light_bishop_;
    delete light_pawn_;
    delete dark_pawn_;
  }

  Board * board_;
  Piece * light_bishop_;
  Piece * light_pawn_;
  Piece * dark_pawn_;
};

TEST_F(BishopTest, MoveToEmptySquare) {
  ASSERT_TRUE(light_bishop_->IsValidMove("Ba1"));
}

TEST_F(BishopTest, CaptureEnemyPiece) {
  ASSERT_TRUE(light_bishop_->IsValidMove("Bxd4"));
}

TEST_F(BishopTest, CannotJumpPiece) {
  ASSERT_FALSE(light_bishop_->IsValidMove("Bh8"));
}

TEST_F(BishopTest, CannotOcupyBusySquare) {
  ASSERT_FALSE(light_bishop_->IsValidMove("Bd2"));
}
