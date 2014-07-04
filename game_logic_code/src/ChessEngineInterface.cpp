/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "ChessEngineInterface.h"
#include "Game.h"
#include "Board.h"
#include "reader/UCIReader.h"

using namespace std;

namespace acortes {
namespace chess {

enum PIPE_FILE_DESC {
  READ_FD = 0,
  WRITE_FD = 1
};

ChessEngineInterface::ChessEngineInterface(string engine_path, bool verbose) :
  engine_path_(engine_path), verbose_(verbose) {
  Initialize();
}

void ChessEngineInterface::Initialize() {
  pipe(parentToChild_);
  pipe(childToParent_);

  // launch stockfish in another thread
  // setup pipes between parent and child
  size_t index = engine_path_.find_last_of('/');

  switch(pid_ = fork()) {
    case -1:
      // fork failed;
      exit(-1);

    case 0: // child launches stockfish
      close(parentToChild_[WRITE_FD]);
      close(childToParent_[READ_FD]);
      dup2(parentToChild_[READ_FD], STDIN_FILENO);
      dup2(childToParent_[WRITE_FD], STDOUT_FILENO);
      dup2(childToParent_[WRITE_FD], STDERR_FILENO);

      execlp(engine_path_.c_str(), engine_path_.substr(index+1).c_str(), NULL);
      exit(-1);

    default: //parent
      //cout << "Child " << pid_ << "running" << endl;
      close(parentToChild_[READ_FD]);
      close(childToParent_[WRITE_FD]);
      break;
  }

  // initial handshaking
  index_current_line_ = 0;
  lines_.clear();

  WaitForLine("Stockfish");
  WriteLine("uci");
  WaitForLine("uciok");
  WriteLine("setoption name Hash value 32");
  WriteLine("isready");
  WaitForLine("readyok");
}

ChessEngineInterface::~ChessEngineInterface() {
  int return_status;
  WriteLine("quit");
  waitpid(pid_, &return_status, 0);
}

size_t ChessEngineInterface::Read() {
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 1000; //1 msec
  size_t readResult = 0;

  // setup for non-blocking reads from child
  FD_ZERO(&readfds_);
  FD_SET(childToParent_[READ_FD], &readfds_);

  switch(select(1+childToParent_[READ_FD], & readfds_, (fd_set*)NULL, (fd_set*)NULL, &timeout)) {
    case 0: // timeout
      break;

    case -1:
      if(errno == EINTR || errno == EAGAIN) {
        errno = 0;
        break;
      } else {
        //cout << "select() failed" << endl;
        exit(-1);
      }

    case 1:
      readResult = read(childToParent_[READ_FD], buffer_, BUFFER_SIZE);
      break;

    default:
      break;
  }

  return readResult;
}

void ChessEngineInterface::ReadLines(vector<string> & lines) {
  while(size_t nbytes = Read()) {
    char * start = buffer_;
    char * current = buffer_;
    char * end_buffer = buffer_ + nbytes;

    while(current < end_buffer) {
      if(*current == '\n') {
        temp_string_.append(start, current);
        lines.push_back(temp_string_);
        //if (verbose_) {
        //  cout << temp_string_ << endl;
        //}
        temp_string_.clear();
        start = current + 1;
      }
      current++;
    }
    temp_string_.append(start,current);
  }
}

std::string ChessEngineInterface::GetNextLine() {
  std::string line = "";

  if(lines_.size() == index_current_line_) {
    // if lines have been processed, try to get more from
    // the engine thread
    ReadLines(lines_);
  }

  if(index_current_line_ < lines_.size()) {
    line = lines_[index_current_line_];
    index_current_line_++;
  }

  return line;
}

void ChessEngineInterface::WaitForLine(string line_start) {
  string line;

  do {
    line = GetNextLine();
  } while(line.empty() || line.compare(0, line_start.size(), line_start) != 0);

}

void ChessEngineInterface::Write(string msg) {
  write(parentToChild_[WRITE_FD], msg.c_str(), msg.size());
}

void ChessEngineInterface::WriteLine(string msg) {
  Write(msg.append("\n"));
}

void ChessEngineInterface::FullAnalysis(Board * board, bool analyze_white, bool analyze_black,
    long time_per_move, long blunder_threshold) {
  while(board != nullptr) {
    // game.Move() changes the state of IsWhiteTurn,
    // is_white_turn stores who is going to move next, then
    // game.Move() performs the move and switch to the next player
    // that's why is_white_turn is stored before making the move
    bool is_white_turn = !board->IsWhiteTurn();

    if((is_white_turn && analyze_white) ||
       (!is_white_turn && analyze_black)) {
      auto engine_option = Analyze(board->previous->FEN(), time_per_move);
      auto player_option = Analyze(board->FEN(), time_per_move);
      auto diff = engine_option.first - player_option.first;

      if((is_white_turn && diff > blunder_threshold) ||
         (!is_white_turn && diff < -blunder_threshold)) {
        board->centipawns = engine_option.first;
        AddAlternative(board, engine_option.second);
      }
    }
    board = board->next;
  }
}

pair<long, string> ChessEngineInterface::Analyze(string fen, long time_secs) {
  WriteLine("ucinewgame");
  WriteLine("position fen " + fen);
  WriteLine("go movetime " + to_string(time_secs*1000));
  WaitForLine("bestmove");

  // get best line
  for(auto it = lines_.rbegin(); it != lines_.rend(); it++) {
    string str = *it;
    size_t index1 = str.find(" score cp ");
    size_t index2 = str.find(" pv ");
    if(index1 != string::npos && index2 != string::npos) {
      index1 += string(" score cp ").length();
      int index1_end = str.find(" ",index1);
      string value_str = str.substr(index1, index1_end - index1);
      long value = atol(value_str.c_str());
      index2 += string(" pv ").length();
      string movements = str.substr(index2);
      return make_pair(value, movements);
    }
  }
  return make_pair(0,"");
}


void ChessEngineInterface::AddAlternative(Board * board, string alternative) {
  UCIReader reader;
  vector<Movement *> moves;

  reader.GetMoves(alternative, moves);
  auto move = moves.begin();
  auto end_move = moves.end();

  // assume there is at least one movement in the alternative
  Board * current_board = new Board(*(board->previous));
  current_board->Move(*move);

  // both share same parent, they are siblings
  current_board->previous = board->previous;
  current_board->next = nullptr;
  current_board->alternative = nullptr;
  board->alternative = current_board;

  for(++move; move != end_move; ++move) {
    // clone existing board, note also next, previous and
    // alternative pointer are shallowed copied
    Board * new_board = new Board(*current_board);
    new_board->Move(*move);
    current_board->next = new_board;
    new_board->previous = current_board;
    new_board->next = nullptr;
    new_board->alternative = nullptr;
    current_board = current_board->next;
  }
}

}
}
