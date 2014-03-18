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
    make_pair("b6", "r1br2k1/p1p1nppp/1pn1p1q1/4P3/2BP4/2N2N2/PP1Q1PPP/R2R2K1 w - - 0 13"),
    make_pair("Bd3", "r1br2k1/p1p1nppp/1pn1p1q1/4P3/3P4/2NB1N2/PP1Q1PPP/R2R2K1 b - - 1 13"),
    make_pair("Qh5", "r1br2k1/p1p1nppp/1pn1p3/4P2q/3P4/2NB1N2/PP1Q1PPP/R2R2K1 w - - 2 14"),
    make_pair("Be4", "r1br2k1/p1p1nppp/1pn1p3/4P2q/3PB3/2N2N2/PP1Q1PPP/R2R2K1 b - - 3 14"),
    make_pair("Rb8", "1rbr2k1/p1p1nppp/1pn1p3/4P2q/3PB3/2N2N2/PP1Q1PPP/R2R2K1 w - - 4 15"),
    make_pair("Nb5", "1rbr2k1/p1p1nppp/1pn1p3/1N2P2q/3PB3/5N2/PP1Q1PPP/R2R2K1 b - - 5 15"),
    make_pair("Rd7", "1rb3k1/p1prnppp/1pn1p3/1N2P2q/3PB3/5N2/PP1Q1PPP/R2R2K1 w - - 6 16"),
    make_pair("Rac1", "1rb3k1/p1prnppp/1pn1p3/1N2P2q/3PB3/5N2/PP1Q1PPP/2RR2K1 b - - 7 16"),
    make_pair("Bb7", "1r4k1/pbprnppp/1pn1p3/1N2P2q/3PB3/5N2/PP1Q1PPP/2RR2K1 w - - 8 17"),
    make_pair("b4", "1r4k1/pbprnppp/1pn1p3/1N2P2q/1P1PB3/5N2/P2Q1PPP/2RR2K1 b - b3 0 17"),
    make_pair("a6", "1r4k1/1bprnppp/ppn1p3/1N2P2q/1P1PB3/5N2/P2Q1PPP/2RR2K1 w - - 0 18"),
    make_pair("Na3", "1r4k1/1bprnppp/ppn1p3/4P2q/1P1PB3/N4N2/P2Q1PPP/2RR2K1 b - - 1 18"),
    make_pair("b5", "1r4k1/1bprnppp/p1n1p3/1p2P2q/1P1PB3/N4N2/P2Q1PPP/2RR2K1 w - - 0 19"),
    make_pair("Nc2", "1r4k1/1bprnppp/p1n1p3/1p2P2q/1P1PB3/5N2/P1NQ1PPP/2RR2K1 b - - 1 19"),
    make_pair("f5", "1r4k1/1bprn1pp/p1n1p3/1p2Pp1q/1P1PB3/5N2/P1NQ1PPP/2RR2K1 w - f6 0 20"),
    make_pair("exf6", "1r4k1/1bprn1pp/p1n1pP2/1p5q/1P1PB3/5N2/P1NQ1PPP/2RR2K1 b - - 0 20"),
    make_pair("gxf6", "1r4k1/1bprn2p/p1n1pp2/1p5q/1P1PB3/5N2/P1NQ1PPP/2RR2K1 w - - 0 21"),
    make_pair("Bxc6", "1r4k1/1bprn2p/p1B1pp2/1p5q/1P1P4/5N2/P1NQ1PPP/2RR2K1 b - - 0 21"),
    make_pair("Bxc6", "1r4k1/2prn2p/p1b1pp2/1p5q/1P1P4/5N2/P1NQ1PPP/2RR2K1 w - - 0 22"),
    make_pair("Qe3", "1r4k1/2prn2p/p1b1pp2/1p5q/1P1P4/4QN2/P1N2PPP/2RR2K1 b - - 1 22"),
    make_pair("Kf7", "1r6/2prnk1p/p1b1pp2/1p5q/1P1P4/4QN2/P1N2PPP/2RR2K1 w - - 2 23"),
    make_pair("a3", "1r6/2prnk1p/p1b1pp2/1p5q/1P1P4/P3QN2/2N2PPP/2RR2K1 b - - 0 23"),
    make_pair("Rg8", "6r1/2prnk1p/p1b1pp2/1p5q/1P1P4/P3QN2/2N2PPP/2RR2K1 w - - 1 24"),
    make_pair("Rd3", "6r1/2prnk1p/p1b1pp2/1p5q/1P1P4/P2RQN2/2N2PPP/2R3K1 b - - 2 24"),
    make_pair("Nf5", "6r1/2pr1k1p/p1b1pp2/1p3n1q/1P1P4/P2RQN2/2N2PPP/2R3K1 w - - 3 25"),
    make_pair("Qe2", "6r1/2pr1k1p/p1b1pp2/1p3n1q/1P1P4/P2R1N2/2N1QPPP/2R3K1 b - - 4 25"),
    make_pair("Qg6", "6r1/2pr1k1p/p1b1ppq1/1p3n2/1P1P4/P2R1N2/2N1QPPP/2R3K1 w - - 5 26"),
    make_pair("g3", "6r1/2pr1k1p/p1b1ppq1/1p3n2/1P1P4/P2R1NP1/2N1QP1P/2R3K1 b - - 0 26"),
    make_pair("Qg4", "6r1/2pr1k1p/p1b1pp2/1p3n2/1P1P2q1/P2R1NP1/2N1QP1P/2R3K1 w - - 1 27"),
    make_pair("Re1", "6r1/2pr1k1p/p1b1pp2/1p3n2/1P1P2q1/P2R1NP1/2N1QP1P/4R1K1 b - - 2 27"),
    make_pair("Bd5", "6r1/2pr1k1p/p3pp2/1p1b1n2/1P1P2q1/P2R1NP1/2N1QP1P/4R1K1 w - - 3 28"),
    make_pair("Ne3", "6r1/2pr1k1p/p3pp2/1p1b1n2/1P1P2q1/P2RNNP1/4QP1P/4R1K1 b - - 4 28"),
    make_pair("Qxf3", "6r1/2pr1k1p/p3pp2/1p1b1n2/1P1P4/P2RNqP1/4QP1P/4R1K1 w - - 0 29"),
    make_pair("Qxf3", "6r1/2pr1k1p/p3pp2/1p1b1n2/1P1P4/P2RNQP1/5P1P/4R1K1 b - - 0 29"),
    make_pair("Bxf3", "6r1/2pr1k1p/p3pp2/1p3n2/1P1P4/P2RNbP1/5P1P/4R1K1 w - - 0 30"),
    make_pair("Nxf5", "6r1/2pr1k1p/p3pp2/1p3N2/1P1P4/P2R1bP1/5P1P/4R1K1 b - - 0 30"),
    make_pair("exf5", "6r1/2pr1k1p/p4p2/1p3p2/1P1P4/P2R1bP1/5P1P/4R1K1 w - - 0 31"),
    make_pair("Rxf3", "6r1/2pr1k1p/p4p2/1p3p2/1P1P4/P4RP1/5P1P/4R1K1 b - - 0 31"),
    make_pair("Rxd4", "6r1/2p2k1p/p4p2/1p3p2/1P1r4/P4RP1/5P1P/4R1K1 w - - 0 32"),
    make_pair("Rxf5", "6r1/2p2k1p/p4p2/1p3R2/1P1r4/P5P1/5P1P/4R1K1 b - - 0 32"),
    make_pair("Rd3", "6r1/2p2k1p/p4p2/1p3R2/1P6/P2r2P1/5P1P/4R1K1 w - - 1 33"),
    make_pair("Re3", "6r1/2p2k1p/p4p2/1p3R2/1P6/P2rR1P1/5P1P/6K1 b - - 2 33"),
    make_pair("Rd1+", "6r1/2p2k1p/p4p2/1p3R2/1P6/P3R1P1/5P1P/3r2K1 w - - 3 34"),
    make_pair("Kg2", "6r1/2p2k1p/p4p2/1p3R2/1P6/P3R1P1/5PKP/3r4 b - - 4 34"),
    make_pair("Rgd8", "3r4/2p2k1p/p4p2/1p3R2/1P6/P3R1P1/5PKP/3r4 w - - 5 35"),
    make_pair("Rc5", "3r4/2p2k1p/p4p2/1pR5/1P6/P3R1P1/5PKP/3r4 b - - 6 35"),
    make_pair("R8d7", "8/2pr1k1p/p4p2/1pR5/1P6/P3R1P1/5PKP/3r4 w - - 7 36"),
    make_pair("Rec3", "8/2pr1k1p/p4p2/1pR5/1P6/P1R3P1/5PKP/3r4 b - - 8 36"),
    make_pair("Kg6", "8/2pr3p/p4pk1/1pR5/1P6/P1R3P1/5PKP/3r4 w - - 9 37"),
    make_pair("Rxc7", "8/2Rr3p/p4pk1/1p6/1P6/P1R3P1/5PKP/3r4 b - - 0 37"),
    make_pair("Rxc7", "8/2r4p/p4pk1/1p6/1P6/P1R3P1/5PKP/3r4 w - - 0 38"),
    make_pair("Rxc7", "8/2R4p/p4pk1/1p6/1P6/P5P1/5PKP/3r4 b - - 0 38"),
    make_pair("Ra1", "8/2R4p/p4pk1/1p6/1P6/P5P1/5PKP/r7 w - - 1 39"),
    make_pair("Rc3", "8/7p/p4pk1/1p6/1P6/P1R3P1/5PKP/r7 b - - 2 39"),
    make_pair("Ra2", "8/7p/p4pk1/1p6/1P6/P1R3P1/r4PKP/8 w - - 3 40"),
    make_pair("h4", "8/7p/p4pk1/1p6/1P5P/P1R3P1/r4PK1/8 b - h3 0 40"),
    make_pair("h5", "8/8/p4pk1/1p5p/1P5P/P1R3P1/r4PK1/8 w - h6 0 41"),
    make_pair("Kf3", "8/8/p4pk1/1p5p/1P5P/P1R2KP1/r4P2/8 b - - 1 41"),
    make_pair("Rd2", "8/8/p4pk1/1p5p/1P5P/P1R2KP1/3r1P2/8 w - - 2 42"),
    make_pair("Ke3", "8/8/p4pk1/1p5p/1P5P/P1R1K1P1/3r1P2/8 b - - 3 42"),
    make_pair("Rd5", "8/8/p4pk1/1p1r3p/1P5P/P1R1K1P1/5P2/8 w - - 4 43"),
    make_pair("Kf4", "8/8/p4pk1/1p1r3p/1P3K1P/P1R3P1/5P2/8 b - - 5 43"),
    make_pair("Rf5+", "8/8/p4pk1/1p3r1p/1P3K1P/P1R3P1/5P2/8 w - - 6 44"),
    make_pair("Ke3", "8/8/p4pk1/1p3r1p/1P5P/P1R1K1P1/5P2/8 b - - 7 44"),
    make_pair("Re5+", "8/8/p4pk1/1p2r2p/1P5P/P1R1K1P1/5P2/8 w - - 8 45"),
};

INSTANTIATE_TEST_CASE_P(
    AllGames,
    TestAllMovements,
    ::testing::Values(Game001));
