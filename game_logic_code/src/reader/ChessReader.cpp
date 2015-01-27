/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include "ChessReader.h"
#include <vector>
#include <string>
#include <memory>

namespace acortes {
namespace chess {

void ChessReader::ParseLine(std::string line, std::vector<std::unique_ptr<Movement>> *moves) {
  std::string move;

  // not very robust assuming each line contains complete moves.
  for (unsigned int i = 0; i < line.size(); ++i) {
    // A move always start with a letter, so just skip
    // spaces, move number and the dot
    while (i < line.size() &&
           (isdigit(line[i]) ||
            isspace(line[i]) ||
            line[i] == '.' ||
            line[i] == '-')) {
      // for now ignore -, it is used at the end to indicate who won
      i++;
    }

    if (i >= line.size()) break;

    move.clear();
    while (i < line.size() && !isspace(line[i])) {
      move += line[i];
      i++;
    }
    moves->push_back(ParseMove(move));
  }
}

}  // namespace chess
}  // namespace acortes
