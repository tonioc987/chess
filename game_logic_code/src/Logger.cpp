#include "Logger.h"
#include <mutex>
#include <fstream>

using namespace std;

namespace acortes {
namespace chess {

class Logger {
private:
  ofstream log;
  mutex mutex_logger;
public:
  Logger() {
    log.open("log.txt");
  }

  ~Logger() {
    log.close();
  }

  void Log(const char *file, int line, const char *message) {
    lock_guard<mutex> guard{mutex_logger};
    log << file << ":" << line << ":" << message << endl;
  }
};

Logger logger;

void Log(const char *file, int line, const char *message) {
  logger.Log(file, line, message);
}

}
}
