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
    make_pair("d4","rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1"),
    make_pair("Nf6","rnbqkb1r/pppppppp/5n2/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 1 2"),
    make_pair("e3","rnbqkb1r/pppppppp/5n2/8/3P4/4P3/PPP2PPP/RNBQKBNR b KQkq - 0 2"),
    make_pair("e6","rnbqkb1r/pppp1ppp/4pn2/8/3P4/4P3/PPP2PPP/RNBQKBNR w KQkq - 0 3"),
    make_pair("Bd3","rnbqkb1r/pppp1ppp/4pn2/8/3P4/3BP3/PPP2PPP/RNBQK1NR b KQkq - 1 3"),
    make_pair("d5","rnbqkb1r/ppp2ppp/4pn2/3p4/3P4/3BP3/PPP2PPP/RNBQK1NR w KQkq d6 0 4"),
    make_pair("f4","rnbqkb1r/ppp2ppp/4pn2/3p4/3P1P2/3BP3/PPP3PP/RNBQK1NR b KQkq f3 0 4"),
    make_pair("a6","rnbqkb1r/1pp2ppp/p3pn2/3p4/3P1P2/3BP3/PPP3PP/RNBQK1NR w KQkq - 0 5"),
    make_pair("Nf3","rnbqkb1r/1pp2ppp/p3pn2/3p4/3P1P2/3BPN2/PPP3PP/RNBQK2R b KQkq - 1 5"),
    make_pair("Nc6","r1bqkb1r/1pp2ppp/p1n1pn2/3p4/3P1P2/3BPN2/PPP3PP/RNBQK2R w KQkq - 2 6"),
    make_pair("c3","r1bqkb1r/1pp2ppp/p1n1pn2/3p4/3P1P2/2PBPN2/PP4PP/RNBQK2R b KQkq - 0 6"),
    make_pair("b5","r1bqkb1r/2p2ppp/p1n1pn2/1p1p4/3P1P2/2PBPN2/PP4PP/RNBQK2R w KQkq b6 0 7"),
    make_pair("O-O","r1bqkb1r/2p2ppp/p1n1pn2/1p1p4/3P1P2/2PBPN2/PP4PP/RNBQ1RK1 b kq - 1 7"),
    make_pair("Bb7","r2qkb1r/1bp2ppp/p1n1pn2/1p1p4/3P1P2/2PBPN2/PP4PP/RNBQ1RK1 w kq - 2 8"),
    make_pair("Nbd2","r2qkb1r/1bp2ppp/p1n1pn2/1p1p4/3P1P2/2PBPN2/PP1N2PP/R1BQ1RK1 b kq - 3 8"),
    make_pair("Ne7","r2qkb1r/1bp1nppp/p3pn2/1p1p4/3P1P2/2PBPN2/PP1N2PP/R1BQ1RK1 w kq - 4 9"),
    make_pair("Ne5","r2qkb1r/1bp1nppp/p3pn2/1p1pN3/3P1P2/2PBP3/PP1N2PP/R1BQ1RK1 b kq - 5 9"),
    make_pair("Ne4","r2qkb1r/1bp1nppp/p3p3/1p1pN3/3PnP2/2PBP3/PP1N2PP/R1BQ1RK1 w kq - 6 10"),
    make_pair("Ndf3","r2qkb1r/1bp1nppp/p3p3/1p1pN3/3PnP2/2PBPN2/PP4PP/R1BQ1RK1 b kq - 7 10")
};

INSTANTIATE_TEST_CASE_P(
    AllGames,
    TestAllMovements,
    ::testing::Values(Game001));
