/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <fstream>
#include <cassert>
#include "UCIReader.h"
#include "Common.h"


namespace acortes {
namespace chess {

UCIReader::UCIReader(std::string moves) {
  std::string move;

  for(unsigned int i = 0; i < moves.size(); ++i) {
    // A move always start with a letter, so just skip
    // spaces, move number and the dot
    while(i < moves.size() &&
           (isdigit(moves[i]) ||
            isspace(moves[i]) ||
            moves[i] == '.' ||
            moves[i] == '-')) {
      // for now ignore -, it is used at the end to indicate who won
      i++;
    }

    if(i >= moves.size()) break;

    move.clear();
    while(i < moves.size() && !isspace(moves[i])) { move += moves[i]; i++; }
    moves_.push_back(ParseMove(move));
  }
}


Movement * UCIReader::ParseMove(std::string move) {
  // right now support just simple movements
  assert(move.size() == 4);
  assert(move[0] >= 'a' && move[0] <= 'h');
  assert(move[1] >= '1' && move[1] <= '8');
  assert(move[2] >= 'a' && move[2] <= 'h');
  assert(move[3] >= '1' && move[3] <= '8');
  Movement * m = new Movement;
  int i = 0;

  m->move = move;

  // process castles first
  /*if(move.compare("O-O") == 0) {
    m->is_short_castle = true;
    m->piece = PieceType::KING;
    return m;
  } else if (move.compare("O-O-O") == 0) {
    m->is_long_castle = true;
    m->piece = PieceType::KING;
    return m;
  }*/

  // it is easier to parse backwards, since it easier to find and
  // process the destination square.

  // 1st process checks or check mate
  /*if(move[i] == '+') {
    m->is_check = true;
    i--;
  } else if(move[i] == '#') {
    m->is_mate = true;
    i--;
  }*/

  // 2nd process source and destination file/rank
  m->source_file = GetFile(move[i]);
  i++;
  m->source_rank = GetRank(move[i]);
  i++;
  m->dest_file = GetFile(move[i]);
  i++;
  m->dest_rank = GetRank(move[i]);
  i++;

  // if it is a simple pawn movement, this entry will be used later
  // otherwise continue processing the string
  /*if(i > 0) {
    i--;
  }*/

  // 4th capture
  /*if(move[i] == 'x') {
    m->is_capture = true;
    i--;
  }*/


  // TODO
  //m->is_promotion = false;

  return m;
}

Movement * UCIReader::GetMove(unsigned int n) const {
  if(n < moves_.size()) {
    return moves_[n];
  } else {
    return nullptr;
  }
}

UCIReader::~UCIReader() {
  for(auto & move : moves_) {
    //delete move;
    //move = nullptr;
  }
}

}
}
