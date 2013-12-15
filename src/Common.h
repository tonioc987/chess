/*
 *  Chess
 *  Copyright (C) 2013  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */
#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <cstdint>

namespace acortes {
namespace chess {

enum class Color{
  Light,
  Dark
};

static inline uint8_t GetFile(std::string move) {
  return static_cast<uint8_t>(move[0]-'a');
}

inline uint8_t GetRank(std::string move) {
  return static_cast<uint8_t>(move[1]-'1');
}

}  // namespace chess
}  // namespace acortes

#endif
