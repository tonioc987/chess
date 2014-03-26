#include <iostream>
#include "Game.h"
#include "Player.h"
#include "PGNPlayer.h"
#include "Board.h"
#include "PGNReader.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "errno.h"

using namespace std;
using namespace acortes::chess;

enum PIPE_FILE_DESC {
  READ_FD = 0,
  WRITE_FD = 1
};

static const int BUFFER_SIZE = 100;

int main()
{
  int parentToChild[2];
  int childToParent[2];
  pid_t pid;
  string dataReadFromChild;
  char buffer[BUFFER_SIZE+1];
  size_t readResult;

  pipe(parentToChild);
  pipe(childToParent);

  switch(pid = fork()) {
    case -1:
      // fork failed;
      exit(-1);
    case 0: // child
      dup2(parentToChild[READ_FD], STDIN_FILENO);
      dup2(childToParent[WRITE_FD], STDOUT_FILENO);
      dup2(childToParent[WRITE_FD], STDERR_FILENO);
      close(parentToChild[WRITE_FD]);
      close(childToParent[READ_FD]);

      execlp("/usr/games/stockfish", "stockfish", NULL);
      exit(-1);

    default: //parent
      cout << "Child " << pid << "running" << endl;
      close(parentToChild[READ_FD]);
      close(childToParent[WRITE_FD]);

      fd_set readfds;
      struct timeval timeout;
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;

      FD_ZERO(&readfds);
      FD_SET(childToParent[READ_FD], &readfds);

      while(true) {
        switch(select(1+childToParent[READ_FD], & readfds, (fd_set*)NULL, (fd_set*)NULL, &timeout)) {
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
            readResult = read(childToParent[READ_FD], buffer, BUFFER_SIZE);
            dataReadFromChild.append(buffer, readResult);
            cout << dataReadFromChild << endl;
            break;

          default:
            exit(-1);
        }
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
