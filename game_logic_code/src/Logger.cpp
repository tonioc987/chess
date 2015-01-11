/* Copyright 2015 Antonio Cortes
 *
 */
#include "Logger.h"
#include <mutex>  // NOLINT
#include <fstream>

namespace acortes {
namespace chess {

class Logger {
 private:
  std::ofstream log;
  std::mutex mutex_logger;
 public:
  Logger() {
    log.open("log.txt");
  }

  ~Logger() {
    log.close();
  }

  void Log(const char *file, int line, const char *message) {
    std::lock_guard<std::mutex> guard{mutex_logger};
    log << file << ":" << line << ":" << message << std::endl;
  }
};

Logger logger;

void Log(const char *file, int line, const char *message) {
  logger.Log(file, line, message);
}

}  // namespace chess
}  // namespace acortes
