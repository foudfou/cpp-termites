#include "WoodChip.hpp"

WoodChip::WoodChip() { }

WoodChip::~WoodChip() { }

bool WoodChip::isWoodChip() const {return true;}

void WoodChip::print(std::ostream& out) const
{
  out << "WoodChip";
}
