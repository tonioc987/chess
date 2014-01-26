#include "Bishop.h"

namespace acortes {
namespace chess {

Bishop::Bishop(Board * board, Color color) : Piece(board, color) {

}

Bishop::~Bishop(){

}

std::string Bishop::LongName() const {
  return "Bishop";
}

std::string Bishop::ShortName() const {
  return "B";
}

bool Bishop::IsValidMove(uint8_t new_file, uint8_t new_rank) const {
  return false;
}

}
}
