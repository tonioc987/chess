/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include "gtest/gtest.h"
#include "Board.h"
#include "Piece.h"
#include "Pawn.h"
using namespace acortes::chess;
using namespace std;

class PawnTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    // light_e_pawn_ has nothing in front of it
    // light_f_pawn_ has another piece right in front of it
    // ight_g_pawn_ has a free square in front of it and then another piece.
    board_ = new Board(8,8);
    light_e_pawn_ = new Pawn(board_, Color::Light);
    light_f_pawn_ = new Pawn(board_, Color::Light);
    light_g_pawn_ = new Pawn(board_, Color::Light);
    dark_f_pawn_ = new Pawn(board_, Color::Dark);
    dark_g_pawn_ = new Pawn(board_, Color::Dark);
    board_->PutPiece(light_e_pawn_, "e2");
    board_->PutPiece(light_f_pawn_, "f2");
    board_->PutPiece(light_g_pawn_, "g2");
    board_->PutPiece(dark_f_pawn_, "f3");
    board_->PutPiece(dark_g_pawn_, "g4");
  }

  virtual void TearDown() {
    delete board_;
    delete light_e_pawn_;
    delete light_f_pawn_;
    delete light_g_pawn_;
    delete dark_f_pawn_;
    delete dark_g_pawn_;
  }

  Board * board_;
  Piece * light_e_pawn_;
  Piece * light_f_pawn_;
  Piece * light_g_pawn_;
  Piece * dark_f_pawn_;
  Piece * dark_g_pawn_;
};

// Pawn moves one square
TEST_F(PawnTest, AdvanceOneSquare) {
  ASSERT_TRUE(light_e_pawn_->IsValidMove("e3"));
}

// Pawn moves two squares in first move
TEST_F(PawnTest, AdvanceTwoSquares) {
  ASSERT_TRUE(light_e_pawn_->IsValidMove("e4"));
}

// Pawn tries to move two squares in second move
TEST_F(PawnTest, AdvanceOneSquareTwoSquares) {
  ASSERT_TRUE(light_e_pawn_->IsValidMove("e3"));
  light_e_pawn_->Move(4,2);
  ASSERT_FALSE(light_e_pawn_->IsValidMove("e5"));
}

// Pawn moves three squares, one at a time
TEST_F(PawnTest, AdvanceOneSquareOneSquareOneSquare) {
  ASSERT_TRUE(light_e_pawn_->IsValidMove("e3"));
  light_e_pawn_->Move(4,2);
  ASSERT_TRUE(light_e_pawn_->IsValidMove("e4"));
  light_e_pawn_->Move(4,3);
  ASSERT_TRUE(light_e_pawn_->IsValidMove("e5"));
  light_e_pawn_->Move(5,4);
}

// Pawn cannot jump over another piece
TEST_F(PawnTest, CannotJumpPieces) {
  ASSERT_FALSE(light_f_pawn_->IsValidMove("f3"));
  ASSERT_FALSE(light_f_pawn_->IsValidMove("f4"));
  ASSERT_TRUE(light_g_pawn_->IsValidMove("g3"));
  ASSERT_FALSE(light_g_pawn_->IsValidMove("g4"));
}

