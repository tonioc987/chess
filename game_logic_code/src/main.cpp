#include <ncurses.h>
#include <getopt.h>
#include <tuple>
#include "Board.h"
#include "ChessEngineInterface.h"

using namespace std;
using namespace acortes::chess;

// Looks like color index 0 of ncurses is reserved or at least cannot be
// configured. That's why the LSBit of this constants is set to 1
// bit 0 always set to 1
// bit 1 is used to select piece color
// bit 2 is used to select square color
#define NCURSES_COLOR_LSB 0x01
#define NCURSES_WHITE_PIECE (0x00 | NCURSES_COLOR_LSB)
#define NCURSES_BLACK_PIECE (0x02 | NCURSES_COLOR_LSB)
#define NCURSES_WHITE_SQUARE (0x00 | NCURSES_COLOR_LSB)
#define NCURSES_BLACK_SQUARE (0x04 | NCURSES_COLOR_LSB)


tuple<string, string,bool,bool,long,long> ParseArguments(int argc, char* argv[]);
string PrintUsage();


void Print(Board & board) {
  char space = ' ';
  char new_line = '\n';
  int color = 0;
  bool isWhiteSquare = true;
  char piece;

  for(auto rank = 8; rank != 0; --rank) {
    for(auto file = 0; file != 8; ++file) {
      color = isWhiteSquare ? NCURSES_WHITE_SQUARE : NCURSES_BLACK_SQUARE;
      piece = board[rank][file];
      if(IsEmpty(piece)) {
        addch(space | COLOR_PAIR(color));
        addch(space | COLOR_PAIR(color));
        addch(space | COLOR_PAIR(color));
      } else {
        color |= isupper(piece) ? NCURSES_WHITE_PIECE : NCURSES_BLACK_PIECE;
        addch(space | COLOR_PAIR(color));
        addch(piece | A_BOLD | COLOR_PAIR(color));
        addch(space | COLOR_PAIR(color));
      }
      isWhiteSquare = !isWhiteSquare;
    }
    addch(new_line);
    isWhiteSquare = !isWhiteSquare;
  }
}


int GameAnalysis(int argc, char* argv[]) {
  int tmp = ' ';
  string engine_path;
  string pgnfile;
  bool analize_light;
  bool analize_dark;
  long time_per_move;
  long blunder_threshold;
  tie(engine_path, pgnfile, analize_light, analize_dark, time_per_move, blunder_threshold) =
      ParseArguments(argc, argv);

  Board * initial_board = Board::CreateFromPGN(pgnfile);
  ChessEngineInterface engine(engine_path, false);
  clear();
  engine.FullAnalysis(initial_board, analize_light, analize_dark, time_per_move, blunder_threshold);

  Board * board = initial_board;

  while(tmp != 'x') {
    clear();
    Print(*board);
    printw("\n\n%s", board->FEN().c_str());
    printw("\n\nMove(%ld): %s", board->centipawns, board->GetMove().c_str());

    if(board->original) {
      printw("\n\nOriginal(%ld): %s", board->original->centipawns, board->original->GetMove().c_str());
    }

    if(board->alternative) {
      printw("\n\nAlternative(%ld): %s", board->alternative->centipawns, board->alternative->GetMove().c_str());
    }

    printw("\n\n");
    refresh();

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
  init_pair(NCURSES_WHITE_PIECE | NCURSES_WHITE_SQUARE, COLOR_YELLOW, COLOR_RED);
  init_pair(NCURSES_WHITE_PIECE | NCURSES_BLACK_SQUARE, COLOR_YELLOW, COLOR_BLACK);
  init_pair(NCURSES_BLACK_PIECE | NCURSES_WHITE_SQUARE, COLOR_CYAN, COLOR_RED);
  init_pair(NCURSES_BLACK_PIECE | NCURSES_BLACK_SQUARE, COLOR_CYAN, COLOR_BLACK);

  GameAnalysis(argc, argv);

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
