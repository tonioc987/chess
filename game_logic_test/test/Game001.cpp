/*  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include "gtest/gtest.h"
#include "Game.h"
#include "Player.h"
#include "PGNPlayer.h"
#include "Board.h"
#include "PGNReader.h"

using namespace std;
using namespace acortes::chess;

class Game001 : public ::testing::Test {
protected:
  virtual void SetUp() {
    board_ = new Board(8,8);
    pgn_ = new PGNReader("/media/common/prgrmmng/chess/game_logic_test/20140212.pgn");
    player1_ = new PGNPlayer(Color::Light, pgn_);
    player2_ = new PGNPlayer(Color::Dark, pgn_);
    game_ = new Game(board_, player1_, player2_);
    game_->InitialSetup();
  }

  virtual void TearDown() {
    delete game_;
    delete player1_;
    delete player2_;
    delete pgn_;
    delete board_;
  }

  Board *board_;
  PGNReader * pgn_;
  PGNPlayer *player1_;
  PGNPlayer *player2_;
  Game * game_;
};

TEST_F(Game001, Game) {
  vector<string> fen = {
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR/",
      "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR/",
      "rnbqkb1r/pppppppp/5n2/8/3P4/8/PPP1PPPP/RNBQKBNR/",
      "rnbqkb1r/pppppppp/5n2/8/3P4/4P3/PPP2PPP/RNBQKBNR/",
      "rnbqkb1r/pppp1ppp/4pn2/8/3P4/4P3/PPP2PPP/RNBQKBNR/",
      "rnbqkb1r/pppp1ppp/4pn2/8/3P4/3BP3/PPP2PPP/RNBQK1NR/",
      "rnbqkb1r/ppp2ppp/4pn2/3p4/3P4/3BP3/PPP2PPP/RNBQK1NR/",
      "rnbqkb1r/ppp2ppp/4pn2/3p4/3P1P2/3BP3/PPP3PP/RNBQK1NR/",
      "rnbqkb1r/1pp2ppp/p3pn2/3p4/3P1P2/3BP3/PPP3PP/RNBQK1NR/",
      "rnbqkb1r/1pp2ppp/p3pn2/3p4/3P1P2/3BPN2/PPP3PP/RNBQK2R/",
      "r1bqkb1r/1pp2ppp/p1n1pn2/3p4/3P1P2/3BPN2/PPP3PP/RNBQK2R/",
      "r1bqkb1r/1pp2ppp/p1n1pn2/3p4/3P1P2/2PBPN2/PP4PP/RNBQK2R/",
      "r1bqkb1r/2p2ppp/p1n1pn2/1p1p4/3P1P2/2PBPN2/PP4PP/RNBQK2R/",
      "r1bqkb1r/2p2ppp/p1n1pn2/1p1p4/3P1P2/2PBPN2/PP4PP/RNBQ1RK1/",// b kq - 1 7
  };

  ASSERT_EQ(fen[0], game_->FEN());

  for(size_t i = 1; i < fen.size(); i++) {
    game_->Move();
    cout << game_->FEN() << endl;
    ASSERT_EQ(fen[i], game_->FEN());
  }
}
