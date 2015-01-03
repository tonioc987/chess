/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef CHESSENGINEINTERFACE_H_
#define CHESSENGINEINTERFACE_H_

#include <queue>
#include <string>
#include <utility>
#include <atomic>

namespace acortes {
namespace chess {

class Board;
class PGNReader;

class ChessEngineInterface {

public:
  ChessEngineInterface(std::string engine_path, bool verbose = false);
  void Initialize();
  void FullAnalysis(Board * board, long time_per_move, long blunder_threshold);
  ~ChessEngineInterface();
  void set_request_stop() { request_stop_ = true; }

private:
  std::string engine_path_;
  int parentToChild_[2];
  int childToParent_[2];
  pid_t pid_;
  static const int BUFFER_SIZE = 1000;
  char buffer_[BUFFER_SIZE+1];
  fd_set readfds_;
  std::string temp_string_;
  std::vector<std::string> lines_;
  size_t index_current_line_;
  bool verbose_;
  std::atomic<bool> request_stop_;

  size_t Read();
  void ReadLines(std::vector<std::string> & lines);
  std::string GetNextLine();
  void ClearLines();
  void WaitForLine(std::string line_start);
  void Write(std::string msg);
  void WriteLine(std::string msg);
  std::pair<long, std::string> Analyze(std::string fen, long time_secs);
};

}
}

#endif
