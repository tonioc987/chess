/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include "gtest/gtest.h"
#include "Board.h"
#include "Piece.h"
#include "King.h"
#include "Pawn.h"
using namespace acortes::chess;
using namespace std;


// 3 . . . . . . . .
// 2 . . . . p P . .
// 1 R . . . K . . R
//   A B C D E F G H
class KingTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    board_ = new Board(8,8);
    light_king_ = new King(board_, Color::Light);
    light_pawn_ = new Pawn(board_, Color::Light);
    dark_pawn_ = new Pawn(board_, Color::Dark);
    board_->PutPiece(light_king_, "e1");
    board_->PutPiece(light_pawn_, "f2");
    board_->PutPiece(dark_pawn_, "e2");
  }

  virtual void TearDown() {
    delete board_;
    delete light_king_;
    delete light_pawn_;
    delete dark_pawn_;
  }

  Board * board_;
  Piece * light_king_;
  Piece * light_pawn_;
  Piece * dark_pawn_;
};


TEST_F(KingTest, MovesToEmptySquares) {
  ASSERT_TRUE(light_king_->IsValidMove("d2"));
}

TEST_F(KingTest, MovesToOcupiedSquare) {
  ASSERT_FALSE(light_king_->IsValidMove("f2"));
}

TEST_F(KingTest, MovesTooFar) {
  ASSERT_FALSE(light_king_->IsValidMove("d3"));
}

TEST_F(KingTest, Captures) {
  ASSERT_TRUE(light_king_->IsValidMove("Kxe2"));
}

TEST_F(KingTest, IllegalCapture) {
  // TODO: to add another black piece to d1 and try to capture
  // not possible because protected by e2 pawn
  ASSERT_FALSE(light_king_->IsValidMove("Kxd1"));
}

TEST_F(KingTest, ShortCastleValid) {
  board_->RemovePiece(dark_pawn_);
  ASSERT_TRUE(light_king_->IsValidMove("o-o"));
}

TEST_F(KingTest, ShortCastleInvValid) {
  ASSERT_FALSE(light_king_->IsValidMove("o-o"));
}

TEST_F(KingTest, LongCastleValid) {
  board_->RemovePiece(dark_pawn_);
  ASSERT_TRUE(light_king_->IsValidMove("o-o-o"));
}

TEST_F(KingTest, LongCastleInvalid) {
  ASSERT_FALSE(light_king_->IsValidMove("o-o"));
}
