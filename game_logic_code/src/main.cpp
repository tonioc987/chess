/* Copyright 2015 Antonio Cortes
 *
 */
#include <ncurses.h>
#include <getopt.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <tuple>
#include <thread>  // NOLINT
#include "Board.h"
#include "ChessEngineInterface.h"
#include "Gui.h"
#include "Logger.h"

using std::string;
using acortes::chess::Log;

struct ConfigParams {
  string engine;
  string pgn_file;
  int time_per_move{1};
  int blunder_threshold{50};
};

int GameAnalysis(int argc, char* argv[]);
void ReadConfigFile(ConfigParams *params);
void ParseArguments(int argc, char* argv[], ConfigParams *params);
string PrintUsage();

int main(int argc, char* argv[]) {
  InitNCurses();
  GameAnalysis(argc, argv);
  EndNCurses();
}

void DestroyBoard(acortes::chess::Board *board) {
  acortes::chess::Board *next;
  while (board) {
    next = board->next;
    delete board;
    board = next;
  }
}

int GameAnalysis(int argc, char* argv[]) {
  using acortes::chess::Board;
  using acortes::chess::ChessEngineInterface;
  int tmp = ' ';
  bool print_as_white = true;
  ConfigParams params;
  ReadConfigFile(&params);
  ParseArguments(argc, argv, &params);

  Board * initial_board = Board::CreateFromPGN(params.pgn_file);
  acortes::chess::ChessEngineInterface engine(params.engine, false);
  // no need to analyze first board, default centipawns = 0 is OK
  std::thread analysis{&ChessEngineInterface::FullAnalysis, &engine,
    initial_board->next, params.time_per_move, params.blunder_threshold};

  acortes::chess::Board * board = initial_board;
  while (tmp != 'x') {
    UpdateMoves(board, nullptr);
    UpdateBoard(*board, print_as_white);
    UpdateFEN(board->FEN());
    tmp = getch();
    if (tmp == KEY_UP && board->previous) {
      board = board->previous;
    } else if (tmp == KEY_DOWN && board->next) {
      board = board->next;
    } else if (tmp == KEY_RIGHT && board->alternative) {
      board = board->alternative;
    } else if (tmp == KEY_LEFT && board->original) {
      board = board->original;
    } else if (tmp == KEY_PPAGE) {
      // find start of alternative or start of game
      while (board->previous) {
        if (board->original) {
          board = board->original;
          break;
        }
        board = board->previous;
      }
    } else if (tmp == 'f') {
      // flip board
      print_as_white = !print_as_white;
    }
  }

  engine.set_request_stop();
  analysis.join();
  DestroyBoard(initial_board);
  return 0;
}

void ReadConfigFile(ConfigParams *config) {
  FILE * file_ptr;
  char key[100];
  char value[100];
  int items;

  file_ptr = fopen(".config", "r");
  if (file_ptr) {
    items = fscanf(file_ptr, "%s %s", key, value);
    while (items == 2) {
      LOG(key);
      LOG(value);
      if (strcmp(key, "engine") == 0) {
        config->engine = string(value);
      } else if (strcmp(key, "time_per_move") == 0) {
        config->time_per_move = atol(value);
      } else if (strcmp(key, "blunder_threshold") == 0) {
        config->blunder_threshold = atol(value);
      }
      items = fscanf(file_ptr, "%s %s", key, value);
    }
    fclose(file_ptr);
  }
}

void ParseArguments(int argc, char * argv[], ConfigParams *params) {
  static struct option long_options[] = {
      {"engine", required_argument, 0, 'e'},
      {"pgnfile", required_argument, 0, 'f'},
      {"time_per_move", required_argument, 0, 't'},
      {"blunder_threshold", required_argument, 0, 'b'}
  };
  int opt = 0;
  int long_index = 0;

  while ((opt = getopt_long(argc, argv, "e:f:t:b:a:",
          long_options, &long_index)) != -1) {
    switch (opt) {
      case 'e': {
        params->engine = string(optarg);
        break;
      }

      case 'f': {
        params->pgn_file = string(optarg);
        break;
      }

      case 't': {
        params->time_per_move = atol(optarg);
        break;
      }

      case 'b': {
        params->blunder_threshold = atol(optarg);
        break;
      }

      default: {
        PrintUsage();
        exit(EXIT_FAILURE);
      }
    }
  }

  if (params->engine.compare("") == 0 || params->pgn_file.compare("") == 0) {
    PrintUsage();
    exit(EXIT_FAILURE);
  }
}

string PrintUsage() {
  return "chess-analyzer --engine=path-to-engine --pgnfile=path-to-pgn "
          "[--time_per_move=seconds] [--blunder_threshold=centipawns]";
}
