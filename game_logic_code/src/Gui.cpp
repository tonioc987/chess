/* Copyright 2015 Antonio Cortes
 *
 */
#include <ncurses.h>
#include <string>
#include <mutex>  // NOLINT
#include "Board.h"
#include "Gui.h"

using std::string;
using acortes::chess::Board;

//    <---------------------- FEN Window  -------------------------->
//
//    +-----------------------------------------+  +---------------+
//    |                                         |  |               |
//    |                                         |  |               |
//    |            Board Window                 |  |               |
//    |                                         |  |               |
//    |                                         |  |   Moves       |
//    +-----------------------------------------+  |   Window      |
//                                                 |               |
//                                                 |               |
//                                                 |               |
//                                                 |               |
//                                                 |               |
//                                                 +---------------+

// Looks like color index 0 of ncurses is reserved or at least cannot be
// configured. That's why the LSBit of this constants is set to 1
// bit 0 always set to 1
// bit 1 is used to select piece color
// bit 2 is used to select square color
#define NCURSES_COLOR_LSB 0x01
#define WHITE_PIECE (0x00 | NCURSES_COLOR_LSB)
#define BLACK_PIECE (0x02 | NCURSES_COLOR_LSB)
#define WHITE_SQUARE (0x00 | NCURSES_COLOR_LSB)
#define BLACK_SQUARE (0x04 | NCURSES_COLOR_LSB)

std::mutex ncurses_mutex;

// Groups data to create a window with ncurses
class NCursesWindow {
 public:
  NCursesWindow(int height, int width, int starty, int startx) :
    height_(height), width_(width), starty_(starty), startx_(startx) {
    window_ = newwin(height, width, starty, startx);
  }

  virtual ~NCursesWindow() {
    delwin(window_);
  }

 protected:
  int height_, width_, starty_, startx_;
  WINDOW *window_;
};

// Displays FEN string at the top of the screen
class FENWindow : public NCursesWindow {
 public:
  FENWindow() :
    NCursesWindow(1, COLS-4, 2, 2) { }

  void Update(string fen) {
    std::lock_guard<std::mutex> lock{ncurses_mutex};
    wclear(window_);
    wprintw(window_, fen.c_str());
    wrefresh(window_);
  }

 private:
};

// Displays chess board below FEN window
class BoardWindow : public NCursesWindow {
 public:
  BoardWindow() :
    NCursesWindow(30, 41, 5, 2) { }

  void Update(const Board & board, const bool print_as_white) {
    using acortes::chess::IsEmpty;
    using acortes::chess::AreSimilarPieces;
    int color = 0;
    bool isWhiteSquare = true;
    int rank_start, rank_end, rank_delta;
    int file_start, file_end, file_delta;
    std::lock_guard<std::mutex> lock{ncurses_mutex};

    if (print_as_white) {
      rank_start = 7;
      rank_end = -1;
      rank_delta = -1;
      file_start = 0;
      file_end = 8;
      file_delta = 1;
    } else {
      rank_start = 0;
      rank_end = 8;
      rank_delta = 1;
      file_start = 7;
      file_end = -1;
      file_delta = -1;
    }

    wclear(window_);
    for (auto rank = rank_start; rank != rank_end; rank += rank_delta) {
      InsertLine(isWhiteSquare);
      for (auto file = file_start; file != file_end; file += file_delta) {
        color = isWhiteSquare ? WHITE_SQUARE : BLACK_SQUARE;
        const char piece = board[rank][file];
        if (IsEmpty(piece)) {
          InsertSpaces(5, color);
        } else {
          color |= isupper(piece) ? WHITE_PIECE : BLACK_PIECE;
          InsertSpaces(2, color);
          waddch(window_,
              piece | (AreSimilarPieces(piece, PAWN) ? 0 : A_BOLD) |
              COLOR_PAIR(color));
          InsertSpaces(2, color);
        }
        isWhiteSquare = !isWhiteSquare;
      }
      waddch(window_, new_line);
      InsertLine(isWhiteSquare);
      isWhiteSquare = !isWhiteSquare;
    }

    wrefresh(window_);
  }

 private:
  static const char space = ' ';
  static const char new_line = '\n';

  void InsertSpaces(const int nspaces, const int color) {
    for (int i = 0; i < nspaces; ++i) {
      waddch(window_, space | COLOR_PAIR(color));
    }
  }

  void InsertLine(bool isWhiteSquare) {
    int color = 0;
    for (int i = 0; i < 8; ++i) {
      color = isWhiteSquare ? WHITE_SQUARE : BLACK_SQUARE;
      InsertSpaces(5, color);
      isWhiteSquare = !isWhiteSquare;
    }
    waddch(window_, new_line);
  }
};

// Displays moves at the right side of the Board window
class MovesWindow : public NCursesWindow {
 public:
  MovesWindow() :
    NCursesWindow(20, 35, 5, 45), board_(nullptr), analysis_(nullptr) { }

  void Update(const Board *const current, const Board *const analysis) {
    int i = 0;
    const Board *temp_board = nullptr;
    const Board *alt_board = nullptr;
    std::lock_guard<std::mutex> lock{ncurses_mutex};

    // board is empty when the chess engine just wants to update
    // the move being analyzed
    if (current) {
      temp_board = board_ = current;
    } else {
      temp_board = board_;
    }

    // this is just updated by the chess engine
    if (analysis) {
      analysis_ = analysis;
    }

    wclear(window_);

    // print just a window of 20 moves (10 before and 10 after)
    i = 0;
    while (temp_board->previous && i < 10) {
      if (temp_board->original) {
        alt_board = temp_board;
      }
      temp_board = temp_board->previous;
      i++;
    }

    if (!temp_board->previous) {
      // head board is an empty move, just skip it
      temp_board = temp_board->next.get();
    }

    i = 0;
    while (temp_board && i < 20) {
      if (!temp_board->IsWhiteTurn()) {
        mvwprintw(window_, i, 0, "%d.", temp_board->move_number()/2+1);
      }

      if (temp_board == board_) wattron(window_, A_STANDOUT);
      if (temp_board == analysis_) wattron(window_, A_BOLD);
      mvwprintw(window_, i, 4, "%s", temp_board->GetMove().c_str());
      if (temp_board->centipawns) {
        mvwprintw(window_, i, 10, "%d", temp_board->centipawns);
      }
      if (temp_board->original) {
        auto temp = temp_board->original;
        mvwprintw(window_, i, 15, "%s", temp->GetMove().c_str());
        mvwprintw(window_, i, 20, "%d", temp->centipawns);
      }
      if (temp_board->alternative) {
        auto alt = temp_board->alternative.get();
        mvwprintw(window_, i, 15, "%s", alt->GetMove().c_str());
        mvwprintw(window_, i, 20, "%d", alt->centipawns);
      }
      if (temp_board == board_) wattroff(window_, A_STANDOUT);
      if (temp_board == analysis_) wattroff(window_, A_BOLD);

      temp_board = temp_board->next.get();
      if (temp_board && temp_board->alternative &&
          temp_board->alternative.get() == alt_board) {
        temp_board = temp_board->alternative.get();
      }
      i++;
    }

    wrefresh(window_);
  }

 private:
  const Board *board_;
  const Board *analysis_;
};

static FENWindow * fen_window;
static BoardWindow * board_window;
static MovesWindow * moves_window;

void InitNCurses() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(WHITE_PIECE|WHITE_SQUARE, COLOR_YELLOW, COLOR_RED);
  init_pair(WHITE_PIECE|BLACK_SQUARE, COLOR_YELLOW, COLOR_BLACK);
  init_pair(BLACK_PIECE|WHITE_SQUARE, COLOR_CYAN, COLOR_RED);
  init_pair(BLACK_PIECE|BLACK_SQUARE, COLOR_CYAN, COLOR_BLACK);
  refresh();  // this initial global refresh is needed
  fen_window = new FENWindow;
  board_window = new BoardWindow;
  moves_window = new MovesWindow;
}

void EndNCurses() {
  delete fen_window;
  delete board_window;
  delete moves_window;
  endwin();
}

void UpdateFEN(const string fen) {
  fen_window->Update(fen);
}

void UpdateBoard(const Board &board, const bool print_as_white) {
  board_window->Update(board, print_as_white);
}

void UpdateMoves(const Board *const current, const Board *const analysis) {
  moves_window->Update(current, analysis);
}
