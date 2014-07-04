/*
 *  Chess
 *  Copyright (C) 2014  A. Cortes
 *  This program is under the terms of the GNU GPL v3
 *  See LICENSE file in the root of this project
 */

#include <fstream>
#include <cassert>
#include "PGNReader.h"
#include "Common.h"

using namespace std;

namespace acortes {
namespace chess {

void PGNReader::GetMoves(string filename, vector<Movement*> & moves) {
  ifstream pgn_file(filename);
  string line;
  string move;

  if(pgn_file.is_open()) {
    while(std::getline(pgn_file, line)) {
      // for now ignore all the information about the game
      if(line.size() > 0 && line[0] != '[') {
        ParseLine(line, moves);
      }
    }
  }

  Color color = Color::WHITE;
  for(auto & movement : moves) {
    movement->color = color;
    color = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
  }

  pgn_file.close();
}


Movement * PGNReader::ParseMove(std::string move) {
  Movement * m = new Movement;
  int i = move.size() - 1;

  m->move = move;

  // process castles first
  if(move.compare("O-O") == 0) {
    m->is_short_castle = true;
    m->piece = PieceType::KING;
    return m;
  } else if (move.compare("O-O-O") == 0) {
    m->is_long_castle = true;
    m->piece = PieceType::KING;
    return m;
  }

  // it is easier to parse backwards, since it easier to find and
  // process the destination square.

  // 1st process checks or check mate
  if(move[i] == '+') {
    m->is_check = true;
    i--;
  } else if(move[i] == '#') {
    m->is_mate = true;
    i--;
  }

  // 2nd process destination rank
  m->dest_rank = GetRank(move[i]);
  i--;

  // 3rd process destination file
  m->dest_file = GetFile(move[i]);

  // if it is a simple pawn movement, this entry will be used later
  // otherwise continue processing the string
  if(i > 0) {
    i--;
  }

  // 4th capture
  if(move[i] == 'x') {
    m->is_capture = true;
    i--;
  }

  // 5th optional disambiguation, in some sense, pawns always have
  // disambiguation file
  switch(move[i]) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h': {
      m->source_file = GetFile(move[i]);
      if(i > 0) {
        i--; // for pawns still this entry will be used later
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
      i--; // not still sure if pawn protection needed as in previous case
      break;
    }
  }

  // 6th piece type
  assert(i==0);
  switch(move[i]) {
    case 'R': {
      m->piece = PieceType::ROOK;
      break;
    }
    case 'B': {
      m->piece = PieceType::BISHOP;
      break;
    }
    case 'N': {
      m->piece = PieceType::KNIGHT;
      break;
    }
    case 'Q': {
      m->piece = PieceType::QUEEN;
      break;
    }
    case 'K': {
      m->piece = PieceType::KING;
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
      m->piece = PieceType::PAWN;
      break;
    }
    default: {
      assert(false);
      break;
    }
  }

  // TODO
  //m->is_enpassant = false;
  //m->is_promotion = false;

  return m;
}

}
}
