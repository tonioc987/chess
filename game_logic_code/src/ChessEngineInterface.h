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

namespace acortes {
namespace chess {

class ChessEngineInterface {

public:
  ChessEngineInterface();
  void ReadLines(std::queue<std::string> & lines);
  void WriteLine(std::string msg);

private:
  int parentToChild_[2];
  int childToParent_[2];
  pid_t pid_;
  static const int BUFFER_SIZE = 100;
  char buffer_[BUFFER_SIZE+1];
  fd_set readfds_;
  std::string temp_string;

  size_t Read();
  void Write(std::string msg);
};

}
}

#endif
