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
#include <utility>
#include <iostream>
#include <fstream>

using namespace std;
using namespace acortes::chess;

class TestAllMovements : public ::testing::TestWithParam<vector<pair<string,string>>> {
protected:
  virtual void SetUp() {
    board_ = new Board(8,8);
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

TEST_P(TestAllMovements, Test) {
  vector<pair<string,string>> movements = GetParam();

  // write to temporal test pgn file
  string filename = "test.pgn";
  ofstream pgn_file;
  pgn_file.open(filename.c_str());
  for(size_t i = 0; i < movements.size(); ++i) {
    if((i%2) == 0) {
      pgn_file << endl;
      pgn_file << (i/2 + 1) << ".";
    }
    pgn_file << movements[i].first << " ";
  }
  pgn_file.close();

  // initialize all variables
  pgn_ = new PGNReader(filename);
  player1_ = new PGNPlayer(Color::Light, pgn_);
  player2_ = new PGNPlayer(Color::Dark, pgn_);
  game_ = new Game(board_, player1_, player2_);
  game_->InitialSetup();

  // starting position
  ASSERT_EQ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", game_->FEN());

  // validate all movements
  for(const auto & move : movements) {
    game_->Move();
    cout << move.first << " --> " << game_->FEN() << endl;
    ASSERT_EQ(move.second, game_->FEN());
  }
};

/*  11.Ng4 Nf5 12.Bxe4 dxe4 13.Nd2 h5 14.Nf2 Nxe3 15.Qb3 Bd5 16.Qd1 Nxd1 17.Rxd1 c5 18.Ndxe4 Bxe4 19.Nxe4 cxd4 20.Rxd4 Qb6
 21.Be3 Qc6 22.Re1 Rd8 23.Red1 Rxd4 24.Rxd4 Be7 25.Kf2 O-O 26.g3 Rd8 27.Nd2 Rxd4 28.Bxd4 Bc5 29.Bxc5 Qxc5+ 30.Kf3 Qd5+
 31.Ke3 e5 32.f5 Qg2 33.Ne4 Qxh2 34.b4 Qxa2 35.Nd6 a5 36.bxa5 Qxa5 37.Kd3 b4 38.Nc4 Qd5+ 39.Kc2 Qxc4 0-1
  */
vector<pair<string,string>> Game001 = {
    make_pair("d4", "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1"),
    make_pair("d5", "rnbqkbnr/ppp1pppp/8/3p4/3P4/8/PPP1PPPP/RNBQKBNR w KQkq d6 0 2"),
    make_pair("c4", "rnbqkbnr/ppp1pppp/8/3p4/2PP4/8/PP2PPPP/RNBQKBNR b KQkq c3 0 2"),
    make_pair("e6", "rnbqkbnr/ppp2ppp/4p3/3p4/2PP4/8/PP2PPPP/RNBQKBNR w KQkq - 0 3"),
    make_pair("Nf3", "rnbqkbnr/ppp2ppp/4p3/3p4/2PP4/5N2/PP2PPPP/RNBQKB1R b KQkq - 1 3"),
    make_pair("Bb4+", "rnbqk1nr/ppp2ppp/4p3/3p4/1bPP4/5N2/PP2PPPP/RNBQKB1R w KQkq - 2 4"),
    make_pair("Bd2", "rnbqk1nr/ppp2ppp/4p3/3p4/1bPP4/5N2/PP1BPPPP/RN1QKB1R b KQkq - 3 4"),
    make_pair("Bxd2+", "rnbqk1nr/ppp2ppp/4p3/3p4/2PP4/5N2/PP1bPPPP/RN1QKB1R w KQkq - 0 5"),
    make_pair("Qxd2", "rnbqk1nr/ppp2ppp/4p3/3p4/2PP4/5N2/PP1QPPPP/RN2KB1R b KQkq - 0 5"),
    make_pair("Qf6", "rnb1k1nr/ppp2ppp/4pq2/3p4/2PP4/5N2/PP1QPPPP/RN2KB1R w KQkq - 1 6"),
    make_pair("e3", "rnb1k1nr/ppp2ppp/4pq2/3p4/2PP4/4PN2/PP1Q1PPP/RN2KB1R b KQkq - 0 6"),
    make_pair("Nc6", "r1b1k1nr/ppp2ppp/2n1pq2/3p4/2PP4/4PN2/PP1Q1PPP/RN2KB1R w KQkq - 1 7"),
    make_pair("Nc3", "r1b1k1nr/ppp2ppp/2n1pq2/3p4/2PP4/2N1PN2/PP1Q1PPP/R3KB1R b KQkq - 2 7"),
    make_pair("dxc4", "r1b1k1nr/ppp2ppp/2n1pq2/8/2pP4/2N1PN2/PP1Q1PPP/R3KB1R w KQkq - 0 8"),
    make_pair("Bxc4", "r1b1k1nr/ppp2ppp/2n1pq2/8/2BP4/2N1PN2/PP1Q1PPP/R3K2R b KQkq - 0 8"),
    make_pair("Nge7", "r1b1k2r/ppp1nppp/2n1pq2/8/2BP4/2N1PN2/PP1Q1PPP/R3K2R w KQkq - 1 9"),
    make_pair("O-O", "r1b1k2r/ppp1nppp/2n1pq2/8/2BP4/2N1PN2/PP1Q1PPP/R4RK1 b kq - 2 9"),
    make_pair("O-O", "r1b2rk1/ppp1nppp/2n1pq2/8/2BP4/2N1PN2/PP1Q1PPP/R4RK1 w - - 3 10"),
    make_pair("e4", "r1b2rk1/ppp1nppp/2n1pq2/8/2BPP3/2N2N2/PP1Q1PPP/R4RK1 b - - 0 10"),
    make_pair("Rd8", "r1br2k1/ppp1nppp/2n1pq2/8/2BPP3/2N2N2/PP1Q1PPP/R4RK1 w - - 1 11"),
    make_pair("e5", "r1br2k1/ppp1nppp/2n1pq2/4P3/2BP4/2N2N2/PP1Q1PPP/R4RK1 b - - 0 11"),
    make_pair("Qg6", "r1br2k1/ppp1nppp/2n1p1q1/4P3/2BP4/2N2N2/PP1Q1PPP/R4RK1 w - - 1 12"),
    make_pair("Rfd1", "r1br2k1/ppp1nppp/2n1p1q1/4P3/2BP4/2N2N2/PP1Q1PPP/R2R2K1 b - - 2 12"),
    make_pair("b6", "r1br2k1/p1p1nppp/1pn1p1q1/4P3/2BP4/2N2N2/PP1Q1PPP/R2R2K1 w - - 0 13")
};

INSTANTIATE_TEST_CASE_P(
    AllGames,
    TestAllMovements,
    ::testing::Values(Game001));
