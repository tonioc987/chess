#include <ncurses.h>
#include <getopt.h>
#include <tuple>
#include <thread>
#include "Board.h"
#include "ChessEngineInterface.h"
#include "Gui.h"

using namespace std;
using namespace acortes::chess;

tuple<string, string,long,long> ParseArguments(int argc, char* argv[]);
string PrintUsage();


int GameAnalysis(int argc, char* argv[]) {
  int tmp = ' ';
  string engine_path;
  string pgnfile;
  bool print_as_white = true;
  long time_per_move;
  long blunder_threshold;
  tie(engine_path, pgnfile, time_per_move, blunder_threshold) =
      ParseArguments(argc, argv);

  Board * initial_board = Board::CreateFromPGN(pgnfile);
  ChessEngineInterface engine(engine_path, false);
  // no need to analyze first board, default centipawns = 0 is OK
  thread analysis{&ChessEngineInterface::FullAnalysis, &engine,
    initial_board->next, time_per_move, blunder_threshold};

  Board * board = initial_board;
  while(tmp != 'x') {
    UpdateMoves(board, nullptr);
    UpdateBoard(*board, print_as_white);
    UpdateFEN(board->FEN());
    tmp = getch();
    if(tmp == KEY_UP && board->previous) {
      board = board->previous;
    }
    else if(tmp == KEY_DOWN && board->next) {
      board = board->next;
    }
    else if(tmp == KEY_RIGHT && board->alternative) {
      board = board->alternative;
    }
    else if(tmp == KEY_LEFT && board->original) {
      board = board->original;
    }
    else if(tmp == KEY_PPAGE) {
      // find start of alternative or start of game
      while(board->previous) {
        if(board->original) {
          board = board->original;
          break;
        }
        board = board->previous;
      }
    }
    else if(tmp == 'f') {
      // flip board
      print_as_white = !print_as_white;
    }
  }

  analysis.join();
  return 0;
}

int main(int argc, char* argv[]) {
	InitNCurses();
  GameAnalysis(argc, argv);
  EndNCurses();
}

tuple<string, string, long, long> ParseArguments(int argc, char * argv[]) {

  string engine = "";
  string pgnfile = "";
  long time_per_move = 1;
  long blunder_threshold = 50;

  static struct option long_options[] = {
      {"engine", required_argument, 0,'e'},
      {"pgnfile", required_argument, 0,'f'},
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

  return make_tuple(engine, pgnfile, time_per_move, blunder_threshold);
}

string PrintUsage() {
  return "chess-analyzer --engine=path-to-engine --pgnfile=path-to-pgn "
          "[--time_per_move=seconds] [--blunder_threshold=centipawns]";
}
