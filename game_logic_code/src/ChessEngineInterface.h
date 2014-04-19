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
#include "Game.h"

namespace acortes {
namespace chess {

class ChessEngineInterface {

public:
  ChessEngineInterface(std::string engine_path, bool verbose = false);
  void Initialize();
  void Analyze(Game game, bool analyze_white, bool analyze_black,
      long time_per_move, long blunder_threshold);

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

  size_t Read();
  void ReadLines(std::vector<std::string> & lines);
  std::string GetNextLine();
  void WaitForLine(std::string line_start);
  void Write(std::string msg);
  void WriteLine(std::string msg);
  std::pair<long, std::string> Analyze(std::string fen, long time_secs);
};

}
}

#endif
