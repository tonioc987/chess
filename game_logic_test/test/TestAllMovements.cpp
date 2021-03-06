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

vector<pair<string,string>> Game002 = {
    make_pair("d4", "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1"),
    make_pair("Nf6", "rnbqkb1r/pppppppp/5n2/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 1 2"),
    make_pair("2.Nc3", "rnbqkb1r/pppppppp/5n2/8/3P4/2N5/PPP1PPPP/R1BQKBNR b KQkq - 2 2"),
    make_pair("e6", "rnbqkb1r/pppp1ppp/4pn2/8/3P4/2N5/PPP1PPPP/R1BQKBNR w KQkq - 0 3"),
    make_pair("3.Bg5", "rnbqkb1r/pppp1ppp/4pn2/6B1/3P4/2N5/PPP1PPPP/R2QKBNR b KQkq - 1 3"),
    make_pair("Bb4", "rnbqk2r/pppp1ppp/4pn2/6B1/1b1P4/2N5/PPP1PPPP/R2QKBNR w KQkq - 2 4"),
    make_pair("4.a3", "rnbqk2r/pppp1ppp/4pn2/6B1/1b1P4/P1N5/1PP1PPPP/R2QKBNR b KQkq - 0 4"),
    make_pair("Bxc3+", "rnbqk2r/pppp1ppp/4pn2/6B1/3P4/P1b5/1PP1PPPP/R2QKBNR w KQkq - 0 5"),
    make_pair("5.bxc3", "rnbqk2r/pppp1ppp/4pn2/6B1/3P4/P1P5/2P1PPPP/R2QKBNR b KQkq - 0 5"),
    make_pair("h6", "rnbqk2r/pppp1pp1/4pn1p/6B1/3P4/P1P5/2P1PPPP/R2QKBNR w KQkq - 0 6"),
    make_pair("6.Bxf6", "rnbqk2r/pppp1pp1/4pB1p/8/3P4/P1P5/2P1PPPP/R2QKBNR b KQkq - 0 6"),
    make_pair("Qxf6", "rnb1k2r/pppp1pp1/4pq1p/8/3P4/P1P5/2P1PPPP/R2QKBNR w KQkq - 0 7"),
    make_pair("7.Nf3", "rnb1k2r/pppp1pp1/4pq1p/8/3P4/P1P2N2/2P1PPPP/R2QKB1R b KQkq - 1 7"),
    make_pair("Nc6", "r1b1k2r/pppp1pp1/2n1pq1p/8/3P4/P1P2N2/2P1PPPP/R2QKB1R w KQkq - 2 8"),
    make_pair("8.e4", "r1b1k2r/pppp1pp1/2n1pq1p/8/3PP3/P1P2N2/2P2PPP/R2QKB1R b KQkq e3 0 8"),
    make_pair("d6", "r1b1k2r/ppp2pp1/2nppq1p/8/3PP3/P1P2N2/2P2PPP/R2QKB1R w KQkq - 0 9"),
    make_pair("9.Bb5", "r1b1k2r/ppp2pp1/2nppq1p/1B6/3PP3/P1P2N2/2P2PPP/R2QK2R b KQkq - 1 9"),
    make_pair("Bd7", "r3k2r/pppb1pp1/2nppq1p/1B6/3PP3/P1P2N2/2P2PPP/R2QK2R w KQkq - 2 10"),
    make_pair("10.O-O", "r3k2r/pppb1pp1/2nppq1p/1B6/3PP3/P1P2N2/2P2PPP/R2Q1RK1 b kq - 3 10"),
    make_pair("a6", "r3k2r/1ppb1pp1/p1nppq1p/1B6/3PP3/P1P2N2/2P2PPP/R2Q1RK1 w kq - 0 11"),
    make_pair("11.e5", "r3k2r/1ppb1pp1/p1nppq1p/1B2P3/3P4/P1P2N2/2P2PPP/R2Q1RK1 b kq - 0 11"),
    make_pair("Qe7", "r3k2r/1ppbqpp1/p1npp2p/1B2P3/3P4/P1P2N2/2P2PPP/R2Q1RK1 w kq - 1 12"),
    make_pair("12.Ba4", "r3k2r/1ppbqpp1/p1npp2p/4P3/B2P4/P1P2N2/2P2PPP/R2Q1RK1 b kq - 2 12"),
    make_pair("b5", "r3k2r/2pbqpp1/p1npp2p/1p2P3/B2P4/P1P2N2/2P2PPP/R2Q1RK1 w kq b6 0 13"),
    make_pair("13.Bb3", "r3k2r/2pbqpp1/p1npp2p/1p2P3/3P4/PBP2N2/2P2PPP/R2Q1RK1 b kq - 1 13"),
    make_pair("O-O", "r4rk1/2pbqpp1/p1npp2p/1p2P3/3P4/PBP2N2/2P2PPP/R2Q1RK1 w - - 2 14"),
    make_pair("14.Re1", "r4rk1/2pbqpp1/p1npp2p/1p2P3/3P4/PBP2N2/2P2PPP/R2QR1K1 b - - 3 14"),
    make_pair("Rad8", "3r1rk1/2pbqpp1/p1npp2p/1p2P3/3P4/PBP2N2/2P2PPP/R2QR1K1 w - - 4 15"),
    make_pair("15.exd6", "3r1rk1/2pbqpp1/p1nPp2p/1p6/3P4/PBP2N2/2P2PPP/R2QR1K1 b - - 0 15"),
    make_pair("cxd6", "3r1rk1/3bqpp1/p1npp2p/1p6/3P4/PBP2N2/2P2PPP/R2QR1K1 w - - 0 16"),
    make_pair("16.d5", "3r1rk1/3bqpp1/p1npp2p/1p1P4/8/PBP2N2/2P2PPP/R2QR1K1 b - - 0 16"),
    make_pair("Na5", "3r1rk1/3bqpp1/p2pp2p/np1P4/8/PBP2N2/2P2PPP/R2QR1K1 w - - 1 17"),
    make_pair("17.Ba2", "3r1rk1/3bqpp1/p2pp2p/np1P4/8/P1P2N2/B1P2PPP/R2QR1K1 b - - 2 17"),
    make_pair("Rc8", "2r2rk1/3bqpp1/p2pp2p/np1P4/8/P1P2N2/B1P2PPP/R2QR1K1 w - - 3 18"),
    make_pair("18.Qd4", "2r2rk1/3bqpp1/p2pp2p/np1P4/3Q4/P1P2N2/B1P2PPP/R3R1K1 b - - 4 18"),
    make_pair("e5", "2r2rk1/3bqpp1/p2p3p/np1Pp3/3Q4/P1P2N2/B1P2PPP/R3R1K1 w - - 0 19"),
    make_pair("19.Qb6", "2r2rk1/3bqpp1/pQ1p3p/np1Pp3/8/P1P2N2/B1P2PPP/R3R1K1 b - - 1 19"),
    make_pair("Nc4", "2r2rk1/3bqpp1/pQ1p3p/1p1Pp3/2n5/P1P2N2/B1P2PPP/R3R1K1 w - - 2 20"),
    make_pair("20.Bxc4", "2r2rk1/3bqpp1/pQ1p3p/1p1Pp3/2B5/P1P2N2/2P2PPP/R3R1K1 b - - 0 20"),
    make_pair("Rxc4", "5rk1/3bqpp1/pQ1p3p/1p1Pp3/2r5/P1P2N2/2P2PPP/R3R1K1 w - - 0 21"),
    make_pair("21.Qxa6", "5rk1/3bqpp1/Q2p3p/1p1Pp3/2r5/P1P2N2/2P2PPP/R3R1K1 b - - 0 21"),
    make_pair("Rxc3", "5rk1/3bqpp1/Q2p3p/1p1Pp3/8/P1r2N2/2P2PPP/R3R1K1 w - - 0 22"),
    make_pair("22.Nd2", "5rk1/3bqpp1/Q2p3p/1p1Pp3/8/P1r5/2PN1PPP/R3R1K1 b - - 1 22"),
    make_pair("Rxc2", "5rk1/3bqpp1/Q2p3p/1p1Pp3/8/P7/2rN1PPP/R3R1K1 w - - 0 23"),
    make_pair("23.Ne4", "5rk1/3bqpp1/Q2p3p/1p1Pp3/4N3/P7/2r2PPP/R3R1K1 b - - 1 23"),
    make_pair("Rc5", "5rk1/3bqpp1/Q2p3p/1prPp3/4N3/P7/5PPP/R3R1K1 w - - 2 24"),
    make_pair("24.Nxc5", "5rk1/3bqpp1/Q2p3p/1pNPp3/8/P7/5PPP/R3R1K1 b - - 0 24"),
    make_pair("dxc5", "5rk1/3bqpp1/Q6p/1ppPp3/8/P7/5PPP/R3R1K1 w - - 0 25"),
    make_pair("25.d6", "5rk1/3bqpp1/Q2P3p/1pp1p3/8/P7/5PPP/R3R1K1 b - - 0 25"),
    make_pair("Qe6", "5rk1/3b1pp1/Q2Pq2p/1pp1p3/8/P7/5PPP/R3R1K1 w - - 1 26"),
    make_pair("26.Re2", "5rk1/3b1pp1/Q2Pq2p/1pp1p3/8/P7/4RPPP/R5K1 b - - 2 26"),
    make_pair("Re8", "4r1k1/3b1pp1/Q2Pq2p/1pp1p3/8/P7/4RPPP/R5K1 w - - 3 27"),
    make_pair("27.Rae1", "4r1k1/3b1pp1/Q2Pq2p/1pp1p3/8/P7/4RPPP/4R1K1 b - - 4 27"),
    make_pair("f6", "4r1k1/3b2p1/Q2Pqp1p/1pp1p3/8/P7/4RPPP/4R1K1 w - - 0 28"),
    make_pair("28.Qb6", "4r1k1/3b2p1/1Q1Pqp1p/1pp1p3/8/P7/4RPPP/4R1K1 b - - 1 28"),
    make_pair("Rc8", "2r3k1/3b2p1/1Q1Pqp1p/1pp1p3/8/P7/4RPPP/4R1K1 w - - 2 29"),
    make_pair("29.f4", "2r3k1/3b2p1/1Q1Pqp1p/1pp1p3/5P2/P7/4R1PP/4R1K1 b - f3 0 29"),
    make_pair("Rc6", "6k1/3b2p1/1QrPqp1p/1pp1p3/5P2/P7/4R1PP/4R1K1 w - - 1 30"),
    make_pair("30.Qxb5", "6k1/3b2p1/2rPqp1p/1Qp1p3/5P2/P7/4R1PP/4R1K1 b - - 0 30"),
    make_pair("Qxd6", "6k1/3b2p1/2rq1p1p/1Qp1p3/5P2/P7/4R1PP/4R1K1 w - - 0 31"),
    make_pair("31.fxe5", "6k1/3b2p1/2rq1p1p/1Qp1P3/8/P7/4R1PP/4R1K1 b - - 0 31"),
    make_pair("fxe5", "6k1/3b2p1/2rq3p/1Qp1p3/8/P7/4R1PP/4R1K1 w - - 0 32"),
    make_pair("32.Qc4+", "6k1/3b2p1/2rq3p/2p1p3/2Q5/P7/4R1PP/4R1K1 b - - 1 32"),
    make_pair("Be6", "6k1/6p1/2rqb2p/2p1p3/2Q5/P7/4R1PP/4R1K1 w - - 2 33"),
    make_pair("33.Qb5", "6k1/6p1/2rqb2p/1Qp1p3/8/P7/4R1PP/4R1K1 b - - 3 33"),
    make_pair("Qd4+", "6k1/6p1/2r1b2p/1Qp1p3/3q4/P7/4R1PP/4R1K1 w - - 4 34"),
    make_pair("34.Kh1", "6k1/6p1/2r1b2p/1Qp1p3/3q4/P7/4R1PP/4R2K b - - 5 34"),
    make_pair("Bd5", "6k1/6p1/2r4p/1Qpbp3/3q4/P7/4R1PP/4R2K w - - 6 35"),
    make_pair("35.Rxe5", "6k1/6p1/2r4p/1QpbR3/3q4/P7/6PP/4R2K b - - 0 35"),
    make_pair("Bxg2+", "6k1/6p1/2r4p/1Qp1R3/3q4/P7/6bP/4R2K w - - 0 36"),
    make_pair("36.Kxg2", "6k1/6p1/2r4p/1Qp1R3/3q4/P7/6KP/4R3 b - - 0 36"),
    make_pair("Rg6+", "6k1/6p1/6rp/1Qp1R3/3q4/P7/6KP/4R3 w - - 1 37"),
    make_pair("37.Kh1", "6k1/6p1/6rp/1Qp1R3/3q4/P7/7P/4R2K b - - 2 37"),
    make_pair("Qf2", "6k1/6p1/6rp/1Qp1R3/8/P7/5q1P/4R2K w - - 3 38"),
    make_pair("38.Qe8+", "4Q1k1/6p1/6rp/2p1R3/8/P7/5q1P/4R2K b - - 4 38"),
    make_pair("Kh7", "4Q3/6pk/6rp/2p1R3/8/P7/5q1P/4R2K w - - 5 39"),
    make_pair("39.Qxg6+", "8/6pk/6Qp/2p1R3/8/P7/5q1P/4R2K b - - 0 39"),
    make_pair("Kxg6", "8/6p1/6kp/2p1R3/8/P7/5q1P/4R2K w - - 0 40"),
    make_pair("40.Re6+", "8/6p1/4R1kp/2p5/8/P7/5q1P/4R2K b - - 1 40"),
    make_pair("Kh7", "8/6pk/4R2p/2p5/8/P7/5q1P/4R2K w - - 2 41"),
    make_pair("41.R6e2", "8/6pk/7p/2p5/8/P7/4Rq1P/4R2K b - - 3 41"),
    make_pair("Qf3+", "8/6pk/7p/2p5/8/P4q2/4R2P/4R2K w - - 4 42"),
    make_pair("42.Kg1", "8/6pk/7p/2p5/8/P4q2/4R2P/4R1K1 b - - 5 42"),
    make_pair("Qxa3", "8/6pk/7p/2p5/8/q7/4R2P/4R1K1 w - - 0 43"),
    make_pair("43.Re3", "8/6pk/7p/2p5/8/q3R3/7P/4R1K1 b - - 1 43"),
    make_pair("Qa5", "8/6pk/7p/q1p5/8/4R3/7P/4R1K1 w - - 2 44"),
    make_pair("44.R3e2", "8/6pk/7p/q1p5/8/8/4R2P/4R1K1 b - - 3 44"),
    make_pair("c4", "8/6pk/7p/q7/2p5/8/4R2P/4R1K1 w - - 0 45"),
    make_pair("45.Re3", "8/6pk/7p/q7/2p5/4R3/7P/4R1K1 b - - 1 45"),
};

vector<pair<string,string>> Game003 = {
    make_pair("1.e4", "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"),
    make_pair("c5", "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"),
    make_pair("2.Nf3", "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"),
    make_pair("d6", "rnbqkbnr/pp2pppp/3p4/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3"),
    make_pair("3.d3", "rnbqkbnr/pp2pppp/3p4/2p5/4P3/3P1N2/PPP2PPP/RNBQKB1R b KQkq - 0 3"),
    make_pair("a6", "rnbqkbnr/1p2pppp/p2p4/2p5/4P3/3P1N2/PPP2PPP/RNBQKB1R w KQkq - 0 4"),
    make_pair("4.g3", "rnbqkbnr/1p2pppp/p2p4/2p5/4P3/3P1NP1/PPP2P1P/RNBQKB1R b KQkq - 0 4"),
    make_pair("Nc6", "r1bqkbnr/1p2pppp/p1np4/2p5/4P3/3P1NP1/PPP2P1P/RNBQKB1R w KQkq - 1 5"),
    make_pair("5.Bg2", "r1bqkbnr/1p2pppp/p1np4/2p5/4P3/3P1NP1/PPP2PBP/RNBQK2R b KQkq - 2 5"),
    make_pair("e6", "r1bqkbnr/1p3ppp/p1npp3/2p5/4P3/3P1NP1/PPP2PBP/RNBQK2R w KQkq - 0 6"),
    make_pair("6.b3", "r1bqkbnr/1p3ppp/p1npp3/2p5/4P3/1P1P1NP1/P1P2PBP/RNBQK2R b KQkq - 0 6"),
    make_pair("Be7", "r1bqk1nr/1p2bppp/p1npp3/2p5/4P3/1P1P1NP1/P1P2PBP/RNBQK2R w KQkq - 1 7"),
    make_pair("7.h4", "r1bqk1nr/1p2bppp/p1npp3/2p5/4P2P/1P1P1NP1/P1P2PB1/RNBQK2R b KQkq h3 0 7"),
    make_pair("Nf6", "r1bqk2r/1p2bppp/p1nppn2/2p5/4P2P/1P1P1NP1/P1P2PB1/RNBQK2R w KQkq - 1 8"),
    make_pair("8.O-O", "r1bqk2r/1p2bppp/p1nppn2/2p5/4P2P/1P1P1NP1/P1P2PB1/RNBQ1RK1 b kq - 2 8"),
    make_pair("O-O", "r1bq1rk1/1p2bppp/p1nppn2/2p5/4P2P/1P1P1NP1/P1P2PB1/RNBQ1RK1 w - - 3 9"),
    make_pair("9.Nc3", "r1bq1rk1/1p2bppp/p1nppn2/2p5/4P2P/1PNP1NP1/P1P2PB1/R1BQ1RK1 b - - 4 9"),
    make_pair("b5", "r1bq1rk1/4bppp/p1nppn2/1pp5/4P2P/1PNP1NP1/P1P2PB1/R1BQ1RK1 w - b6 0 10"),
    make_pair("10.Bb2", "r1bq1rk1/4bppp/p1nppn2/1pp5/4P2P/1PNP1NP1/PBP2PB1/R2Q1RK1 b - - 1 10"),
    make_pair("Bb7", "r2q1rk1/1b2bppp/p1nppn2/1pp5/4P2P/1PNP1NP1/PBP2PB1/R2Q1RK1 w - - 2 11"),
    make_pair("11.Ne2", "r2q1rk1/1b2bppp/p1nppn2/1pp5/4P2P/1P1P1NP1/PBP1NPB1/R2Q1RK1 b - - 3 11"),
    make_pair("Qc7", "r4rk1/1bq1bppp/p1nppn2/1pp5/4P2P/1P1P1NP1/PBP1NPB1/R2Q1RK1 w - - 4 12"),
    make_pair("12.Qd2", "r4rk1/1bq1bppp/p1nppn2/1pp5/4P2P/1P1P1NP1/PBPQNPB1/R4RK1 b - - 5 12"),
    make_pair("Rad8", "3r1rk1/1bq1bppp/p1nppn2/1pp5/4P2P/1P1P1NP1/PBPQNPB1/R4RK1 w - - 6 13"),
    make_pair("13.c3", "3r1rk1/1bq1bppp/p1nppn2/1pp5/4P2P/1PPP1NP1/PB1QNPB1/R4RK1 b - - 0 13"),
    make_pair("Rfe8", "3rr1k1/1bq1bppp/p1nppn2/1pp5/4P2P/1PPP1NP1/PB1QNPB1/R4RK1 w - - 1 14"),
    make_pair("14.d4", "3rr1k1/1bq1bppp/p1nppn2/1pp5/3PP2P/1PP2NP1/PB1QNPB1/R4RK1 b - - 0 14"),
    make_pair("cxd4", "3rr1k1/1bq1bppp/p1nppn2/1p6/3pP2P/1PP2NP1/PB1QNPB1/R4RK1 w - - 0 15"),
    make_pair("15.cxd4", "3rr1k1/1bq1bppp/p1nppn2/1p6/3PP2P/1P3NP1/PB1QNPB1/R4RK1 b - - 0 15"),
    make_pair("d5", "3rr1k1/1bq1bppp/p1n1pn2/1p1p4/3PP2P/1P3NP1/PB1QNPB1/R4RK1 w - - 0 16"),
    make_pair("16.e5", "3rr1k1/1bq1bppp/p1n1pn2/1p1pP3/3P3P/1P3NP1/PB1QNPB1/R4RK1 b - - 0 16"),
    make_pair("Ne4", "3rr1k1/1bq1bppp/p1n1p3/1p1pP3/3Pn2P/1P3NP1/PB1QNPB1/R4RK1 w - - 1 17"),
    make_pair("17.Qd3", "3rr1k1/1bq1bppp/p1n1p3/1p1pP3/3Pn2P/1P1Q1NP1/PB2NPB1/R4RK1 b - - 2 17"),
    make_pair("Nb4", "3rr1k1/1bq1bppp/p3p3/1p1pP3/1n1Pn2P/1P1Q1NP1/PB2NPB1/R4RK1 w - - 3 18"),
    make_pair("18.Qe3", "3rr1k1/1bq1bppp/p3p3/1p1pP3/1n1Pn2P/1P2QNP1/PB2NPB1/R4RK1 b - - 4 18"),
    make_pair("Nc2", "3rr1k1/1bq1bppp/p3p3/1p1pP3/3Pn2P/1P2QNP1/PBn1NPB1/R4RK1 w - - 5 19"),
    make_pair("19.Rac1", "3rr1k1/1bq1bppp/p3p3/1p1pP3/3Pn2P/1P2QNP1/PBn1NPB1/2R2RK1 b - - 6 19"),
    make_pair("Nxe3", "3rr1k1/1bq1bppp/p3p3/1p1pP3/3Pn2P/1P2nNP1/PB2NPB1/2R2RK1 w - - 0 20"),
    make_pair("20.Rxc7", "3rr1k1/1bR1bppp/p3p3/1p1pP3/3Pn2P/1P2nNP1/PB2NPB1/5RK1 b - - 0 20"),
    make_pair("Nxf1", "3rr1k1/1bR1bppp/p3p3/1p1pP3/3Pn2P/1P3NP1/PB2NPB1/5nK1 w - - 0 21"),
    make_pair("21.Rxb7", "3rr1k1/1R2bppp/p3p3/1p1pP3/3Pn2P/1P3NP1/PB2NPB1/5nK1 b - - 0 21"),
    make_pair("Nfd2", "3rr1k1/1R2bppp/p3p3/1p1pP3/3Pn2P/1P3NP1/PB1nNPB1/6K1 w - - 1 22"),
    make_pair("22.Nxd2", "3rr1k1/1R2bppp/p3p3/1p1pP3/3Pn2P/1P4P1/PB1NNPB1/6K1 b - - 0 22"),
    make_pair("Nxd2", "3rr1k1/1R2bppp/p3p3/1p1pP3/3P3P/1P4P1/PB1nNPB1/6K1 w - - 0 23"),
    make_pair("23.Bc3", "3rr1k1/1R2bppp/p3p3/1p1pP3/3P3P/1PB3P1/P2nNPB1/6K1 b - - 1 23"),
    make_pair("Ne4", "3rr1k1/1R2bppp/p3p3/1p1pP3/3Pn2P/1PB3P1/P3NPB1/6K1 w - - 2 24"),
    make_pair("24.Bxe4", "3rr1k1/1R2bppp/p3p3/1p1pP3/3PB2P/1PB3P1/P3NP2/6K1 b - - 0 24"),
    make_pair("dxe4", "3rr1k1/1R2bppp/p3p3/1p2P3/3Pp2P/1PB3P1/P3NP2/6K1 w - - 0 25"),
    make_pair("25.Kg2", "3rr1k1/1R2bppp/p3p3/1p2P3/3Pp2P/1PB3P1/P3NPK1/8 b - - 1 25"),
    make_pair("Ba3", "3rr1k1/1R3ppp/p3p3/1p2P3/3Pp2P/bPB3P1/P3NPK1/8 w - - 2 26"),
    make_pair("26.Ra7", "3rr1k1/R4ppp/p3p3/1p2P3/3Pp2P/bPB3P1/P3NPK1/8 b - - 3 26"),
    make_pair("Ra8", "r3r1k1/R4ppp/p3p3/1p2P3/3Pp2P/bPB3P1/P3NPK1/8 w - - 4 27"),
    make_pair("27.Rb7", "r3r1k1/1R3ppp/p3p3/1p2P3/3Pp2P/bPB3P1/P3NPK1/8 b - - 5 27"),
    make_pair("Re7", "r5k1/1R2rppp/p3p3/1p2P3/3Pp2P/bPB3P1/P3NPK1/8 w - - 6 28"),
    make_pair("28.Rb6", "r5k1/4rppp/pR2p3/1p2P3/3Pp2P/bPB3P1/P3NPK1/8 b - - 7 28"),
    make_pair("Rc7", "r5k1/2r2ppp/pR2p3/1p2P3/3Pp2P/bPB3P1/P3NPK1/8 w - - 8 29"),
    make_pair("29.f3", "r5k1/2r2ppp/pR2p3/1p2P3/3Pp2P/bPB2PP1/P3N1K1/8 b - - 0 29"),
    make_pair("Rac8", "2r3k1/2r2ppp/pR2p3/1p2P3/3Pp2P/bPB2PP1/P3N1K1/8 w - - 1 30"),
    make_pair("30.Rxa6", "2r3k1/2r2ppp/R3p3/1p2P3/3Pp2P/bPB2PP1/P3N1K1/8 b - - 0 30"),
    make_pair("b4", "2r3k1/2r2ppp/R3p3/4P3/1p1Pp2P/bPB2PP1/P3N1K1/8 w - - 0 31"),
    make_pair("31.Ba1", "2r3k1/2r2ppp/R3p3/4P3/1p1Pp2P/bP3PP1/P3N1K1/B7 b - - 1 31"),
    make_pair("Rc2", "2r3k1/5ppp/R3p3/4P3/1p1Pp2P/bP3PP1/P1r1N1K1/B7 w - - 2 32"),
    make_pair("32.Kf2", "2r3k1/5ppp/R3p3/4P3/1p1Pp2P/bP3PP1/P1r1NK2/B7 b - - 3 32"),
    make_pair("Rxa2", "2r3k1/5ppp/R3p3/4P3/1p1Pp2P/bP3PP1/r3NK2/B7 w - - 0 33"),
    make_pair("33.fxe4", "2r3k1/5ppp/R3p3/4P3/1p1PP2P/bP4P1/r3NK2/B7 b - - 0 33"),
    make_pair("Rxa1", "2r3k1/5ppp/R3p3/4P3/1p1PP2P/bP4P1/4NK2/r7 w - - 0 34"),
    make_pair("34.d5", "2r3k1/5ppp/R3p3/3PP3/1p2P2P/bP4P1/4NK2/r7 b - - 0 34"),
    make_pair("exd5", "2r3k1/5ppp/R7/3pP3/1p2P2P/bP4P1/4NK2/r7 w - - 0 35"),
    make_pair("35.exd5", "2r3k1/5ppp/R7/3PP3/1p5P/bP4P1/4NK2/r7 b - - 0 35"),
    make_pair("Ra2", "2r3k1/5ppp/R7/3PP3/1p5P/bP4P1/r3NK2/8 w - - 1 36"),
    make_pair("36.d6", "2r3k1/5ppp/R2P4/4P3/1p5P/bP4P1/r3NK2/8 b - - 0 36"),
    make_pair("Rd2", "2r3k1/5ppp/R2P4/4P3/1p5P/bP4P1/3rNK2/8 w - - 1 37"),
    make_pair("37.Ke3", "2r3k1/5ppp/R2P4/4P3/1p5P/bP2K1P1/3rN3/8 b - - 2 37"),
    make_pair("Rd5", "2r3k1/5ppp/R2P4/3rP3/1p5P/bP2K1P1/4N3/8 w - - 3 38"),
    make_pair("38.Ke4", "2r3k1/5ppp/R2P4/3rP3/1p2K2P/bP4P1/4N3/8 b - - 4 38"),
    make_pair("Rd1", "2r3k1/5ppp/R2P4/4P3/1p2K2P/bP4P1/4N3/3r4 w - - 5 39"),
    make_pair("39.Ra7", "2r3k1/R4ppp/3P4/4P3/1p2K2P/bP4P1/4N3/3r4 b - - 6 39"),
    make_pair("Rd8", "3r2k1/R4ppp/3P4/4P3/1p2K2P/bP4P1/4N3/3r4 w - - 7 40"),
    make_pair("40.Nd4", "3r2k1/R4ppp/3P4/4P3/1p1NK2P/bP4P1/8/3r4 b - - 8 40"),
    make_pair("Re1+", "3r2k1/R4ppp/3P4/4P3/1p1NK2P/bP4P1/8/4r3 w - - 9 41"),
    make_pair("41.Kd5", "3r2k1/R4ppp/3P4/3KP3/1p1N3P/bP4P1/8/4r3 b - - 10 41"),
    make_pair("Rd1", "3r2k1/R4ppp/3P4/3KP3/1p1N3P/bP4P1/8/3r4 w - - 11 42"),
    make_pair("42.Re7", "3r2k1/4Rppp/3P4/3KP3/1p1N3P/bP4P1/8/3r4 b - - 12 42"),
    make_pair("Kf8", "3r1k2/4Rppp/3P4/3KP3/1p1N3P/bP4P1/8/3r4 w - - 13 43"),
    make_pair("43.Rc7", "3r1k2/2R2ppp/3P4/3KP3/1p1N3P/bP4P1/8/3r4 b - - 14 43"),
    make_pair("Ke8", "3rk3/2R2ppp/3P4/3KP3/1p1N3P/bP4P1/8/3r4 w - - 15 44"),
    make_pair("44.Kc5", "3rk3/2R2ppp/3P4/2K1P3/1p1N3P/bP4P1/8/3r4 b - - 16 44"),
    make_pair("Rc1+", "3rk3/2R2ppp/3P4/2K1P3/1p1N3P/bP4P1/8/2r5 w - - 17 45"),
    make_pair("45.Kb6", "3rk3/2R2ppp/1K1P4/4P3/1p1N3P/bP4P1/8/2r5 b - - 18 45"),
    make_pair("Rb8+", "1r2k3/2R2ppp/1K1P4/4P3/1p1N3P/bP4P1/8/2r5 w - - 19 46"),
    make_pair("46.Rb7", "1r2k3/1R3ppp/1K1P4/4P3/1p1N3P/bP4P1/8/2r5 b - - 20 46"),
    make_pair("Rxb7+", "4k3/1r3ppp/1K1P4/4P3/1p1N3P/bP4P1/8/2r5 w - - 0 47"),
    make_pair("47.Kxb7", "4k3/1K3ppp/3P4/4P3/1p1N3P/bP4P1/8/2r5 b - - 0 47"),
    make_pair("Kd7", "8/1K1k1ppp/3P4/4P3/1p1N3P/bP4P1/8/2r5 w - - 1 48"),
    make_pair("48.Kb6", "8/3k1ppp/1K1P4/4P3/1p1N3P/bP4P1/8/2r5 b - - 2 48"),
    make_pair("Rc3", "8/3k1ppp/1K1P4/4P3/1p1N3P/bPr3P1/8/8 w - - 3 49"),
    make_pair("49.Kb5", "8/3k1ppp/3P4/1K2P3/1p1N3P/bPr3P1/8/8 b - - 4 49"),
    make_pair("Rxg3", "8/3k1ppp/3P4/1K2P3/1p1N3P/bP4r1/8/8 w - - 0 50"),
    make_pair("50.Kb6", "8/3k1ppp/1K1P4/4P3/1p1N3P/bP4r1/8/8 b - - 1 50"),
    make_pair("Rg4", "8/3k1ppp/1K1P4/4P3/1p1N2rP/bP6/8/8 w - - 2 51"),
    make_pair("51.e6+", "8/3k1ppp/1K1PP3/8/1p1N2rP/bP6/8/8 b - - 0 51"),
    make_pair("fxe6", "8/3k2pp/1K1Pp3/8/1p1N2rP/bP6/8/8 w - - 0 52"),
    make_pair("52.Nxe6", "8/3k2pp/1K1PN3/8/1p4rP/bP6/8/8 b - - 0 52"),
    make_pair("Kxe6", "8/6pp/1K1Pk3/8/1p4rP/bP6/8/8 w - - 0 53"),
    make_pair("53.Kc7", "8/2K3pp/3Pk3/8/1p4rP/bP6/8/8 b - - 1 53"),
    make_pair("Rg6", "8/2K3pp/3Pk1r1/8/1p5P/bP6/8/8 w - - 2 54"),
    make_pair("54.d7", "8/2KP2pp/4k1r1/8/1p5P/bP6/8/8 b - - 0 54"),
};

vector<pair<string,string>> Game004 = {
    make_pair("1.d4", "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1"),
    make_pair("d5", "rnbqkbnr/ppp1pppp/8/3p4/3P4/8/PPP1PPPP/RNBQKBNR w KQkq d6 0 2"),
    make_pair("2.c4", "rnbqkbnr/ppp1pppp/8/3p4/2PP4/8/PP2PPPP/RNBQKBNR b KQkq c3 0 2"),
    make_pair("e6", "rnbqkbnr/ppp2ppp/4p3/3p4/2PP4/8/PP2PPPP/RNBQKBNR w KQkq - 0 3"),
    make_pair("3.Nc3", "rnbqkbnr/ppp2ppp/4p3/3p4/2PP4/2N5/PP2PPPP/R1BQKBNR b KQkq - 1 3"),
    make_pair("Bb4", "rnbqk1nr/ppp2ppp/4p3/3p4/1bPP4/2N5/PP2PPPP/R1BQKBNR w KQkq - 2 4"),
    make_pair("4.a3", "rnbqk1nr/ppp2ppp/4p3/3p4/1bPP4/P1N5/1P2PPPP/R1BQKBNR b KQkq - 0 4"),
    make_pair("Bxc3+", "rnbqk1nr/ppp2ppp/4p3/3p4/2PP4/P1b5/1P2PPPP/R1BQKBNR w KQkq - 0 5"),
    make_pair("5.bxc3", "rnbqk1nr/ppp2ppp/4p3/3p4/2PP4/P1P5/4PPPP/R1BQKBNR b KQkq - 0 5"),
    make_pair("dxc4", "rnbqk1nr/ppp2ppp/4p3/8/2pP4/P1P5/4PPPP/R1BQKBNR w KQkq - 0 6"),
    make_pair("6.e3", "rnbqk1nr/ppp2ppp/4p3/8/2pP4/P1P1P3/5PPP/R1BQKBNR b KQkq - 0 6"),
    make_pair("b5", "rnbqk1nr/p1p2ppp/4p3/1p6/2pP4/P1P1P3/5PPP/R1BQKBNR w KQkq b6 0 7"),
    make_pair("7.a4", "rnbqk1nr/p1p2ppp/4p3/1p6/P1pP4/2P1P3/5PPP/R1BQKBNR b KQkq - 0 7"),
    make_pair("c6", "rnbqk1nr/p4ppp/2p1p3/1p6/P1pP4/2P1P3/5PPP/R1BQKBNR w KQkq - 0 8"),
    make_pair("8.axb5", "rnbqk1nr/p4ppp/2p1p3/1P6/2pP4/2P1P3/5PPP/R1BQKBNR b KQkq - 0 8"),
    make_pair("cxb5", "rnbqk1nr/p4ppp/4p3/1p6/2pP4/2P1P3/5PPP/R1BQKBNR w KQkq - 0 9"),
    make_pair("9.Qf3", "rnbqk1nr/p4ppp/4p3/1p6/2pP4/2P1PQ2/5PPP/R1B1KBNR b KQkq - 1 9"),
    make_pair("Qd5", "rnb1k1nr/p4ppp/4p3/1p1q4/2pP4/2P1PQ2/5PPP/R1B1KBNR w KQkq - 2 10"),
    make_pair("10.e4", "rnb1k1nr/p4ppp/4p3/1p1q4/2pPP3/2P2Q2/5PPP/R1B1KBNR b KQkq - 0 10"),
    make_pair("Qc6", "rnb1k1nr/p4ppp/2q1p3/1p6/2pPP3/2P2Q2/5PPP/R1B1KBNR w KQkq - 1 11"),
    make_pair("11.d5", "rnb1k1nr/p4ppp/2q1p3/1p1P4/2p1P3/2P2Q2/5PPP/R1B1KBNR b KQkq - 0 11"),
    make_pair("exd5", "rnb1k1nr/p4ppp/2q5/1p1p4/2p1P3/2P2Q2/5PPP/R1B1KBNR w KQkq - 0 12"),
    make_pair("12.exd5", "rnb1k1nr/p4ppp/2q5/1p1P4/2p5/2P2Q2/5PPP/R1B1KBNR b KQkq - 0 12"),
    make_pair("Qf6", "rnb1k1nr/p4ppp/5q2/1p1P4/2p5/2P2Q2/5PPP/R1B1KBNR w KQkq - 1 13"),
    make_pair("13.Qe4+", "rnb1k1nr/p4ppp/5q2/1p1P4/2p1Q3/2P5/5PPP/R1B1KBNR b KQkq - 2 13"),
    make_pair("Qe7", "rnb1k1nr/p3qppp/8/1p1P4/2p1Q3/2P5/5PPP/R1B1KBNR w KQkq - 3 14"),
    make_pair("14.Qxe7+", "rnb1k1nr/p3Qppp/8/1p1P4/2p5/2P5/5PPP/R1B1KBNR b KQkq - 0 14"),
    make_pair("Nxe7", "rnb1k2r/p3nppp/8/1p1P4/2p5/2P5/5PPP/R1B1KBNR w KQkq - 0 15"),
    make_pair("15.d6", "rnb1k2r/p3nppp/3P4/1p6/2p5/2P5/5PPP/R1B1KBNR b KQkq - 0 15"),
    make_pair("Nf5", "rnb1k2r/p4ppp/3P4/1p3n2/2p5/2P5/5PPP/R1B1KBNR w KQkq - 1 16"),
    make_pair("16.Bf4", "rnb1k2r/p4ppp/3P4/1p3n2/2p2B2/2P5/5PPP/R3KBNR b KQkq - 2 16"),
    make_pair("Bb7", "rn2k2r/pb3ppp/3P4/1p3n2/2p2B2/2P5/5PPP/R3KBNR w KQkq - 3 17"),
    make_pair("17.f3", "rn2k2r/pb3ppp/3P4/1p3n2/2p2B2/2P2P2/6PP/R3KBNR b KQkq - 0 17"),
    make_pair("h5", "rn2k2r/pb3pp1/3P4/1p3n1p/2p2B2/2P2P2/6PP/R3KBNR w KQkq h6 0 18"),
    make_pair("18.Ne2", "rn2k2r/pb3pp1/3P4/1p3n1p/2p2B2/2P2P2/4N1PP/R3KB1R b KQkq - 1 18"),
    make_pair("O-O", "rn3rk1/pb3pp1/3P4/1p3n1p/2p2B2/2P2P2/4N1PP/R3KB1R w KQ - 2 19"),
    make_pair("19.O-O-O", "rn3rk1/pb3pp1/3P4/1p3n1p/2p2B2/2P2P2/4N1PP/2KR1B1R b - - 3 19"),
    make_pair("Rd8", "rn1r2k1/pb3pp1/3P4/1p3n1p/2p2B2/2P2P2/4N1PP/2KR1B1R w - - 4 20"),
    make_pair("20.Ng3", "rn1r2k1/pb3pp1/3P4/1p3n1p/2p2B2/2P2PN1/6PP/2KR1B1R b - - 5 20"),
    make_pair("Nxg3", "rn1r2k1/pb3pp1/3P4/1p5p/2p2B2/2P2Pn1/6PP/2KR1B1R w - - 0 21"),
    make_pair("21.hxg3", "rn1r2k1/pb3pp1/3P4/1p5p/2p2B2/2P2PP1/6P1/2KR1B1R b - - 0 21"),
    make_pair("g6", "rn1r2k1/pb3p2/3P2p1/1p5p/2p2B2/2P2PP1/6P1/2KR1B1R w - - 0 22"),
    make_pair("22.g4", "rn1r2k1/pb3p2/3P2p1/1p5p/2p2BP1/2P2P2/6P1/2KR1B1R b - - 0 22"),
    make_pair("hxg4", "rn1r2k1/pb3p2/3P2p1/1p6/2p2Bp1/2P2P2/6P1/2KR1B1R w - - 0 23"),
    make_pair("23.fxg4", "rn1r2k1/pb3p2/3P2p1/1p6/2p2BP1/2P5/6P1/2KR1B1R b - - 0 23"),
    make_pair("f6", "rn1r2k1/pb6/3P1pp1/1p6/2p2BP1/2P5/6P1/2KR1B1R w - - 0 24"),
    make_pair("24.Rh6","rn1r2k1/pb6/3P1ppR/1p6/2p2BP1/2P5/6P1/2KR1B2 b - - 1 24"),
    make_pair("Kg7","rn1r4/pb4k1/3P1ppR/1p6/2p2BP1/2P5/6P1/2KR1B2 w - - 2 25"),
    make_pair("25.g5","rn1r4/pb4k1/3P1ppR/1p4P1/2p2B2/2P5/6P1/2KR1B2 b - - 0 25"),
    make_pair("fxg5","rn1r4/pb4k1/3P2pR/1p4p1/2p2B2/2P5/6P1/2KR1B2 w - - 0 26"),
    make_pair("26.Bxg5","rn1r4/pb4k1/3P2pR/1p4B1/2p5/2P5/6P1/2KR1B2 b - - 0 26"),
    make_pair("Rd7","rn6/pb1r2k1/3P2pR/1p4B1/2p5/2P5/6P1/2KR1B2 w - - 1 27"),
    make_pair("27.g3","rn6/pb1r2k1/3P2pR/1p4B1/2p5/2P3P1/8/2KR1B2 b - - 0 27"),
    make_pair("Nc6","r7/pb1r2k1/2nP2pR/1p4B1/2p5/2P3P1/8/2KR1B2 w - - 1 28"),
    make_pair("28.Bh3","r7/pb1r2k1/2nP2pR/1p4B1/2p5/2P3PB/8/2KR4 b - - 2 28"),
    make_pair("Rf7","r7/pb3rk1/2nP2pR/1p4B1/2p5/2P3PB/8/2KR4 w - - 3 29"),
    make_pair("29.Be6","r7/pb3rk1/2nPB1pR/1p4B1/2p5/2P3P1/8/2KR4 b - - 4 29"),
    make_pair("Rf3","r7/pb4k1/2nPB1pR/1p4B1/2p5/2P2rP1/8/2KR4 w - - 5 30"),
    make_pair("30.Rh7+","r7/pb4kR/2nPB1p1/1p4B1/2p5/2P2rP1/8/2KR4 b - - 6 30"),
    make_pair("Kxh7","r7/pb5k/2nPB1p1/1p4B1/2p5/2P2rP1/8/2KR4 w - - 0 31"),
    make_pair("31.Rh1+","r7/pb5k/2nPB1p1/1p4B1/2p5/2P2rP1/8/2K4R b - - 1 31"),
    make_pair("Kg7","r7/pb4k1/2nPB1p1/1p4B1/2p5/2P2rP1/8/2K4R w - - 2 32"),
    make_pair("32.Bh6+","r7/pb4k1/2nPB1pB/1p6/2p5/2P2rP1/8/2K4R b - - 3 32"),
    make_pair("Kf6","r7/pb6/2nPBkpB/1p6/2p5/2P2rP1/8/2K4R w - - 4 33"),
};

INSTANTIATE_TEST_CASE_P(
    AllGames,
    TestAllMovements,
    ::testing::Values(Game001, Game002, Game003, Game004));

