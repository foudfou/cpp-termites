#include "Termite.hpp"

Termite::Termite(const TermiteSpeciesPtr& spc) : species(spc) {}

Termite::~Termite() {}

bool Termite::isTermite() const {return true;}

std::string Termite::dump() const {return "Termite";}
