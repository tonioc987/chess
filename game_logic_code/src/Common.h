/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef COMMON_H_
#define COMMON_H_

#include <string>

namespace acortes {
namespace chess {

enum class Color{
  Light,
  Dark
};

inline int GetFile(char file) {
  return static_cast<int>(file -'a');
}

inline int GetRank(char rank) {
  return static_cast<int>(rank-'1');
}

inline char GetFile(int file) {
  return static_cast<char>(file + 'a');
}

inline char GetRank(int rank) {
  return static_cast<char>(rank + '1');
}

}  // namespace chess
}  // namespace acortes

#endif
