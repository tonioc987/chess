#include <iostream>
#include "Game.h"
#include "Player.h"
#include "PGNPlayer.h"
#include "Board.h"
#include "PGNReader.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "errno.h"
#include <queue>

using namespace std;
using namespace acortes::chess;

enum PIPE_FILE_DESC {
  READ_FD = 0,
  WRITE_FD = 1
};

static const int BUFFER_SIZE = 100;

namespace acortes {
namespace chess {

class ChessEngineInterface {

public:
  ChessEngineInterface();
  void ReadLines(queue<string> & lines);
  void WriteLine(string msg);

private:
  int parentToChild_[2];
  int childToParent_[2];
  pid_t pid_;
  char buffer_[BUFFER_SIZE+1];
  fd_set readfds_;
  string temp_string;

  size_t Read();
  void Write(string msg);
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
      cout << "Child " << pid_ << "running" << endl;
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
        cout << "select() failed" << endl;
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

int main()
{
  ChessEngineInterface engine;
  queue<string> lines;
  sleep(1);

  engine.WriteLine("uci");
  while(true) {
    engine.ReadLines(lines);
    while(lines.size()) {
      cout << ">>>" << lines.front() << endl;
      lines.pop();
    }
  }

  /*Board *board = new Board(8,8);
  PGNReader pgn("/home/acp/Downloads/parayuri_vs_rdom_2014_03_23.pgn");
  PGNPlayer *player1 = new PGNPlayer(Color::Light, &pgn);
  PGNPlayer *player2 = new PGNPlayer(Color::Dark, &pgn);

  Game game(board, player1, player2);
  game.InitialSetup();
  while(game.Move()) {
    cout << game.FEN() << endl;
  }*/

  return 0;
}
