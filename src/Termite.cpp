#include <stdexcept>
#include "Termite.hpp"

Termite::Termite(const TermiteSpeciesPtr& spc) : species(spc) {}

Termite::~Termite() {}

bool Termite::isTermite() const {return true;}

std::string Termite::dump() const {return "Termite";}

TermiteSpeciesPtr Termite::getSpecies() const {return species;}

bool Termite::isLoaded() const {return bool(woodChip);}

void Termite::load(const PiecePtr& chip)
{
  if (woodChip)
    throw std::logic_error("Termite already loaded");
  if (!chip->isWoodChip())
    throw std::invalid_argument("Piece to be loaded is not a WoodChip");
  woodChip = chip;
}

PiecePtr Termite::unload()
{
  // XXX: could swap help here ?
  PiecePtr tmpw = woodChip;
  woodChip = nullptr;
  return tmpw;
}
