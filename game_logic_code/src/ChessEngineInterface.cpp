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
#include <cassert>
#include "ChessEngineInterface.h"
#include "Board.h"
#include "reader/UCIReader.h"
#include "Gui.h"
#include "Logger.h"

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
  ClearLines();
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
        LOG(temp_string_.c_str());
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

void ChessEngineInterface::ClearLines() {
  index_current_line_ = 0;
  lines_.clear();
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

    UpdateMoves(nullptr, board);

    // for current move, evaluation of best reply
    LOG(">>>>>>>> New move <<<<<<<<<<");
    LOG(board->GetMove().c_str());
    LOG(board->FEN().c_str());
    auto analysis = Analyze(board->FEN(), time_per_move);

    // engine evaluations are always from its viewpoint
    // normalize them to + white advantage, - black advantage
    // just need to inverse black evaluations
    if(!white_to_move) {
      analysis.first *= -1;
    }
    board->centipawns = analysis.first;
    LOG(to_string(board->centipawns).c_str());
    LOG(analysis.second.c_str());

    if(board->previous) {
      auto diff = board->previous->centipawns - board->centipawns;

      if((!white_to_move && diff > blunder_threshold) ||
         (white_to_move && diff < -blunder_threshold)) {
        LOG(to_string(diff).c_str());
        Board::AddAlternative(board, alternative_str);
        LOG(board->FEN().c_str());
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

  auto it = lines_.rbegin();
  auto it_end = lines_.rend();
  static const string bestmove_pattern = "bestmove ";
  static const string mate_pattern = " score mate ";
  static const string cp_pattern = " score cp ";
  static const string pv_pattern = " pv ";

  for(; it != it_end; ++it) {
    size_t index = it->find(bestmove_pattern);
    if(index != string::npos) {
      index += bestmove_pattern.length();
      string candidate_move = it->substr(index,
          it->find(" ", index)-index);
      if(candidate_move.compare("(none)") == 0) {
        // in case of checkmate, there is no reply
        return make_pair(0,"");
      } else {
        // move on to look for the best move
        break;
      }
    }
  }

  // get best line
  for(; it != it_end; ++it) {
    long value = 0;
    const string &str = *it;
    const string *pattern = &cp_pattern;
    size_t index1 = str.find(*pattern);
    if(index1 == string::npos) {
      pattern = &mate_pattern;
      index1 = str.find(*pattern);
    }

    if(index1 != string::npos) {
      index1 += pattern->length();
      string value_str = str.substr(index1,
          str.find(" ", index1) - index1);
      value = atol(value_str.c_str());
      if(pattern->compare(mate_pattern) == 0) {
        // very high evaluation for mate
        value = (value < 0) ? -100000 : 100000;
      }
    }

    size_t index2 = str.find(pv_pattern);
    if(index1 != string::npos && index2 != string::npos) {
      index2 += pv_pattern.length();
      string movements = str.substr(index2);
      ClearLines();
      return make_pair(value, movements);
    }
  }
  assert(false);
  return make_pair(0,"");
}


}
}
