#include "Piece.hpp"

Piece::Piece() { }

Piece::~Piece() { }

bool Piece::isWoodChip() const {return false;}

bool Piece::isTermite() const {return false;}

void Piece::print(std::ostream& out) const
{
  out << "Piece";
}
