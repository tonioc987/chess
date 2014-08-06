/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <ncurses.h>
#include "ChessEngineInterface.h"
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


void ChessEngineInterface::FullAnalysis(Board * board, long time_per_move, long blunder_threshold) {

  string alternative_str = "";

  while(board != nullptr) {
    // IsWhiteTurn stores who is going to move next
    // true => black just moved
    //      => white to move
    //      => engine evaluation from white perspective
    //      => can evaluate black movement
    // false => white just moved
    //       => black to move
    //       => engine evaluation from black perspective
    //       => can evaluate white movement
    bool white_to_move = board->IsWhiteTurn();

    // for current move, evaluation of best reply
    auto analysis = Analyze(board->FEN(), time_per_move);

    // engine evaluations are always from its viewpoint
    // normalize them to + white advantage, - black advantage
    // just need to inverse black evaluations
    if(!white_to_move) {
      analysis.first *= -1;
    }

    board->centipawns = analysis.first;
    clear();
    printw("\n%s",board->GetMove().c_str());
    printw("\n%s",alternative_str.c_str());
    refresh();

    if(board->previous) {
      auto diff = board->previous->centipawns - board->centipawns;

      if((!white_to_move && diff > blunder_threshold) ||
         (white_to_move && diff < -blunder_threshold)) {
        Board::AddAlternative(board, alternative_str);
        board->alternative->centipawns = board->previous->centipawns;
      }
    }
    board = board->next;
    alternative_str = analysis.second;
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


}
}
