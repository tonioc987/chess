/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include "UCIReader.h"

namespace acortes {
namespace chess {


std::unique_ptr<Movement> UCIReader::ParseMove(std::string move) {
  // right now support just simple movements
  assert(move.size() == 4 || move.size() == 5);
  assert(move[0] >= 'a' && move[0] <= 'h');
  assert(move[1] >= '1' && move[1] <= '8');
  assert(move[2] >= 'a' && move[2] <= 'h');
  assert(move[3] >= '1' && move[3] <= '8');
  std::unique_ptr<Movement> m(new Movement);
  int i = 0;

  m->move = move;

  // 2nd process source and destination file/rank
  m->source_file = GetFile(move[i]);
  i++;
  m->source_rank = GetRank(move[i]);
  i++;
  m->dest_file = GetFile(move[i]);
  i++;
  m->dest_rank = GetRank(move[i]);
  i++;

  if (move.size() == 5) {
    m->promoted_piece = move[i];
  }

  return m;
}

void UCIReader::GetMoves(std::string moves_str,
    std::vector<std::unique_ptr<Movement>> *moves) {
  ParseLine(moves_str, moves);
}

}  // namespace chess
}  // namespace acortes
