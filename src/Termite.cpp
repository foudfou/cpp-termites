#include "Termite.hpp"

Termite::Termite() { }

Termite::~Termite() { }

bool Termite::isTermite() const {return true;}

void Termite::print(std::ostream& out) const
{
  out << "Termite" << std::endl;
}
