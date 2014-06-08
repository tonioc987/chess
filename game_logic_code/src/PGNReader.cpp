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
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"


namespace acortes {
namespace chess {

PGNReader::PGNReader(std::string filename) {
  std::ifstream pgn_file(filename);
  std::string line;
  std::string move;
  Color color = Color::WHITE;

  if(pgn_file.is_open()) {
    while(std::getline(pgn_file, line)) {
      // for now ignore all the information about the game
      if(line.size() > 0 && line[0] != '[') {
        // not very robust assuming each line contains complete moves.
        for(unsigned int i = 0; i < line.size(); ++i) {
          // A move always start with a letter, so just skip
          // spaces, move number and the dot
          while(i < line.size() &&
                 (isdigit(line[i]) ||
                  isspace(line[i]) ||
                  line[i] == '.' ||
                  line[i] == '-')) {
            // for now ignore -, it is used at the end to indicate who won
            i++;
          }

          if(i >= line.size()) break;

          move.clear();
          while(i < line.size() && !isspace(line[i])) { move += line[i]; i++; }
          moves_.push_back(ParseMove(move, color));
          color = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
        }
      }
    }
  }

  pgn_file.close();
}


Movement * PGNReader::ParseMove(std::string move, Color color) {
  Movement * m = new Movement;
  int i = move.size() - 1;

  m->move = move;
  m->color = color;

  // process castles first
  if(move.compare("O-O") == 0) {
    m->is_short_castle = true;
    return m;
  } else if (move.compare("O-O-O") == 0) {
    m->is_long_castle = true;
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

Movement * PGNReader::GetMove(unsigned int n) const {
  if(n < moves_.size()) {
    return moves_[n];
  } else {
    return nullptr;
  }
}

PGNReader::~PGNReader() {
  for(auto & move : moves_) {
    delete move;
    move = nullptr;
  }
}

}
}
