#include <ncurses.h>
#include <getopt.h>
#include <tuple>
#include "Game.h"
#include "PGNReader.h"
#include "ChessEngineInterface.h"

using namespace std;
using namespace acortes::chess;

tuple<string, string,bool,bool,long,long> ParseArguments(int argc, char* argv[]);
string PrintUsage();

int GameAnalysis(int argc, char* argv[]) {
  string engine_path;
  string pgnfile;
  bool analize_light;
  bool analize_dark;
  long time_per_move;
  long blunder_threshold;
  tie(engine_path, pgnfile, analize_light, analize_dark, time_per_move, blunder_threshold) =
      ParseArguments(argc, argv);

  PGNReader pgn(pgnfile);
  Game game;
  game.InitialSetup();

  ChessEngineInterface engine(engine_path, false);
  engine.Analyze(game, pgn, analize_light, analize_dark, time_per_move, blunder_threshold);

  return 0;
}

struct ChessGameState {
  struct ChessGameState * next;
  struct ChessGameState * prev;
  struct ChessGameState * alternatives;
  string move;
  string FEN;

  ChessGameState() :
    next(nullptr), prev(nullptr), alternatives(nullptr) {}
  ChessGameState(string move, string FEN) :
    next(nullptr), prev(nullptr), alternatives(nullptr),
    move(move), FEN(FEN) {}
};

void PrintFEN(string FEN) {
  char space = ' ';
  char new_line = '\n';
  int num_spaces = 0;

  for(char & c : FEN) {
    if(c == ' ') {
      break;
    }
    else if(c == '/') {
      addch(new_line);
    }
    else if(c > '0' && c <= '9') {
      num_spaces = c - '0';
      for(int i = 0; i < num_spaces; ++i) {
        addch('.' | A_BOLD | COLOR_PAIR(3));
        addch(space);
      }
    }
    else if(isupper(c)) {
      addch(c | A_BOLD | COLOR_PAIR(1));
      addch(space);
    }
    else {
      addch(c | A_BOLD | COLOR_PAIR(2));
      addch(space);
    }
  }
}

int DisplayGame(int argc, char* argv[]) {
  string pgnfile = string(argv[1]);
  int tmp = ' ';
  int current_move = 0;
  Movement * move = nullptr;

  PGNReader pgn(pgnfile);
  Game game;
  game.InitialSetup();

  ChessGameState * start_game = new ChessGameState;
  ChessGameState * last_move = start_game;

  while((move = pgn.GetMove(current_move))) {
    last_move->next = new ChessGameState(game.GetLastMove(), game.FEN());
    last_move->next->prev = last_move;
    last_move = last_move->next;
  }

  last_move = start_game->next;
  while(tmp != 'x') {
    clear();
    PrintFEN(last_move->FEN);
    refresh();
    tmp = getch();
    if(tmp == KEY_LEFT) {
      if(last_move->prev != start_game) {
        last_move = last_move->prev;
      }
    }
    else if (tmp == KEY_RIGHT) {
      if(last_move->next) {
        last_move = last_move->next;
      }
    }
  }

  return 0;
}

int main(int argc, char* argv[]) {
  // ncurses initialization
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_WHITE, COLOR_BLACK);

  DisplayGame(argc, argv);

  // ncurses clean up
  endwin();
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

string PrintUsage() {
  return "chess-analyzer --engine=path-to-engine --pgnfile=path-to-pgn [--analize_light] "
          "[--analize-dark] [--time_per_move=seconds] [--blunder_threshold=centipawns]";
}
