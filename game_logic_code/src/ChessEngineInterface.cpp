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

using namespace std;

namespace acortes {
namespace chess {

enum PIPE_FILE_DESC {
  READ_FD = 0,
  WRITE_FD = 1
};

ChessEngineInterface::ChessEngineInterface() {
  pipe(parentToChild_);
  pipe(childToParent_);

  switch(pid_ = fork()) {
    case -1:
      // fork failed;
      exit(-1);

    case 0: // child launches stockfish
      dup2(parentToChild_[READ_FD], STDIN_FILENO);
      dup2(childToParent_[WRITE_FD], STDOUT_FILENO);
      dup2(childToParent_[WRITE_FD], STDERR_FILENO);
      close(parentToChild_[WRITE_FD]);
      close(childToParent_[READ_FD]);

      execlp("/usr/games/stockfish", "stockfish", NULL);
      exit(-1);

    default: //parent
      //cout << "Child " << pid_ << "running" << endl;
      close(parentToChild_[READ_FD]);
      close(childToParent_[WRITE_FD]);

      // setup for non-blocking reads from child
      FD_ZERO(&readfds_);
      FD_SET(childToParent_[READ_FD], &readfds_);
      break;
  }
}

size_t ChessEngineInterface::Read() {
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 1000; //1 msec
  size_t readResult = 0;

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

void ChessEngineInterface::ReadLines(queue<string> & lines) {
  while(size_t nbytes = Read()) {
    char * start = buffer_;
    char * current = buffer_;
    char * end_buffer = buffer_ + nbytes;

    while(current < end_buffer) {
      if(*current == '\n') {
        temp_string.append(start, current);
        lines.push(temp_string);
        temp_string.clear();
        start = current + 1;
      }
      current++;
    }
    temp_string.append(start,current);
  }
}

void ChessEngineInterface::Write(string msg) {
  write(parentToChild_[WRITE_FD], msg.c_str(), msg.size());
}

void ChessEngineInterface::WriteLine(string msg) {
  Write(msg.append("\n"));
}

}
}
