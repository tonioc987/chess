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
#include "PGNReader.h"

namespace acortes {
namespace chess {

void PGNReader::GetMoves(std::string filename, std::vector<std::unique_ptr<Movement>> *moves) {
  std::ifstream pgn_file(filename);
  std::string line;
  std::string move;

  if (pgn_file.is_open()) {
    while (std::getline(pgn_file, line)) {
      // for now ignore all the information about the game
      if (line.size() > 0 && line[0] != '[') {
        ParseLine(line, moves);
      }
    }
  }

  bool isWhite = true;
  for (auto & movement : *moves) {
    if (!isWhite) {
      movement->piece = Black(movement->piece);
      if (!IsEmpty(movement->promoted_piece)) {
        movement->promoted_piece = Black(movement->promoted_piece);
      }
    }
    isWhite = !isWhite;
  }

  pgn_file.close();
}


std::unique_ptr<Movement> PGNReader::ParseMove(std::string move) {
  std::unique_ptr<Movement> m(new Movement);
  int i = move.size() - 1;

  m->move = move;

  // process castles first
  if (move.compare("O-O") == 0) {
    m->is_short_castle = true;
    m->piece = KING;
    return m;
  } else if (move.compare("O-O-O") == 0) {
    m->is_long_castle = true;
    m->piece = KING;
    return m;
  }

  // it is easier to parse backwards, since it easier to find and
  // process the destination square.

  // 1st process checks or check mate
  if (move[i] == '+') {
    m->is_check = true;
    i--;
  } else if (move[i] == '#') {
    m->is_mate = true;
    i--;
  }

  // process promotion e.g. e8=Q
  switch (move[i]) {
    case 'R':
    case 'B':
    case 'N':
    case 'Q': {
      m->promoted_piece = move[i];
      i--;
      assert('=' == move[i]);
      i--;
      break;
    }
  }


  // 2nd process destination rank
  m->dest_rank = GetRank(move[i]);
  i--;

  // 3rd process destination file
  m->dest_file = GetFile(move[i]);

  // if it is a simple pawn movement, this entry will be used later
  // otherwise continue processing the string
  if (i > 0) {
    i--;
  }

  // 4th capture
  if (move[i] == 'x') {
    m->is_capture = true;
    i--;
  }

  // 5th optional disambiguation, in some sense, pawns always have
  // disambiguation file
  switch (move[i]) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h': {
      m->source_file = GetFile(move[i]);
      if (i > 0) {
        i--;  // for pawns still this entry will be used later
      }
      break;
    }

    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8': {
      m->source_rank = GetRank(move[i]);
      i--;  // not still sure if pawn protection needed as in previous case
      break;
    }
  }

  // 6th piece type
  assert(i == 0);
  switch (move[i]) {
    case 'R':
    case 'B':
    case 'N':
    case 'Q':
    case 'K': {
      m->piece = move[i];
      break;
    }

    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h': {
      m->piece = PAWN;
      break;
    }
    default: {
      assert(false);
      break;
    }
  }

  // TODO(acortes):
  // m->is_enpassant = false;
  // m->is_promotion = false;

  return m;
}

}  // namespace chess
}  // namespace acortes
