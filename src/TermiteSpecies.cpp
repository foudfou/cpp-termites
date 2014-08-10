#include <algorithm>
#include "helpers.hpp"
#include "TermiteSpecies.hpp"

TermiteSpecies::TermiteSpecies(const std::string& nam,
                               const std::list<WoodSpeciesPtr>& wds)
  : name(nam), woodsLiked(wds)
{}

TermiteSpecies::~TermiteSpecies() { }

std::string TermiteSpecies::getName() const {return name;}

bool TermiteSpecies::likes(const std::string& wood) const
{
  return bool(tmt::find(woodsLiked, wood));
}
