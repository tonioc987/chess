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
  ChessEngineInterface(bool initialize);
  void Initialize();

private:
  int parentToChild_[2];
  int childToParent_[2];
  pid_t pid_;
  static const int BUFFER_SIZE = 1000;
  char buffer_[BUFFER_SIZE+1];
  fd_set readfds_;
  std::string temp_string;
  std::queue<std::string> lines;

  size_t Read();
  void ReadLines(std::queue<std::string> & lines);
  std::string ReadLine();
  void Write(std::string msg);
  void WriteLine(std::string msg);
};

}
}

#endif
