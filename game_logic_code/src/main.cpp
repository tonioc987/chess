#include <iostream>
#include <getopt.h>
#include <tuple>
#include "Game.h"
#include "Player.h"
#include "PGNPlayer.h"
#include "Board.h"
#include "PGNReader.h"
#include "ChessEngineInterface.h"

using namespace std;
using namespace acortes::chess;

tuple<string, string,bool,bool,long,long> ParseArguments(int argc, char* argv[]);
void PrintUsage();

int main(int argc, char* argv[])
{
  string engine_path;
  string pgnfile;
  bool analize_light;
  bool analize_dark;
  long time_per_move;
  long blunder_threshold;
  tie(engine_path, pgnfile, analize_light, analize_dark, time_per_move, blunder_threshold) =
      ParseArguments(argc, argv);

  Board *board = new Board(8,8);
  PGNReader pgn(pgnfile);
  PGNPlayer *player1 = new PGNPlayer(Color::Light, &pgn);
  PGNPlayer *player2 = new PGNPlayer(Color::Dark, &pgn);
  Game game(board, player1, player2);
  game.InitialSetup();

  ChessEngineInterface engine(engine_path, false);
  engine.Analyze(game, analize_light, analize_dark, time_per_move, blunder_threshold);

  delete player1;
  delete player2;
  delete board;

  return 0;
}

tuple<string, string, bool, bool, long, long> ParseArguments(int argc, char * argv[]) {

  string engine = "";
  string pgnfile = "";
  bool analize_light = false;
  bool analize_dark = false;
  long time_per_move = 1;
  long blunder_threshold = 50;

  static struct option long_options[] = {
      {"engine", required_argument, 0,'e'},
      {"pgnfile", required_argument, 0,'f'},
      {"analyze_light", no_argument,0,'l'},
      {"analyze_dark", no_argument, 0, 'd'},
      {"time_per_move", required_argument, 0, 't'},
      {"blunder_threshold", required_argument,0,'b'}
  };

  int opt=0;
  int long_index = 0;

  while((opt = getopt_long(argc, argv, "e:f:t:b:a:",
          long_options, &long_index)) != -1) {
    switch(opt) {

      case 'e': {
        engine = string(optarg);
        break;
      }

      case 'f': {
        pgnfile = string(optarg);
        break;
      }

      case 'l': {
        analize_light = true;
        break;
      }

      case 'd': {
        analize_dark = true;
        break;
      }

      case 't': {
        time_per_move = atol(optarg);
        break;
      }

      case 'b': {
        blunder_threshold = atol(optarg);
        break;
      }

      default: {
        PrintUsage();
        exit(EXIT_FAILURE);
      }
    }
  }

  if(engine.compare("") == 0 || pgnfile.compare("") == 0) {
    PrintUsage();
    exit(EXIT_FAILURE);
  }

  if(!analize_light && !analize_dark) {
    analize_light = true;
  }

  return make_tuple(engine, pgnfile, analize_light, analize_dark, time_per_move, blunder_threshold);
}

void PrintUsage() {
  cout << "chess-analyzer --engine=path-to-engine --pgnfile=path-to-pgn [--analize_light] "
       << "[--analize-dark] [--time_per_move=seconds] [--blunder_threshold=centipawns]"
       << endl;
}
