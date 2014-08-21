#include <algorithm>
#include "helpers.hpp"
#include "TermiteSpecies.hpp"

TermiteSpecies::TermiteSpecies(const std::string& nam,
                               const std::list<WoodSpeciesPtr>& wds)
  : name(nam), woodsLiked(wds)
{}

TermiteSpecies::~TermiteSpecies() { }

std::string TermiteSpecies::getName() const {return name;}

bool TermiteSpecies::likes(const WoodSpeciesPtr& woodSpecies) const
{
  bool found = false;
  for (auto tspc : woodsLiked) {
    if (tspc == woodSpecies)
    {
      found = true;
      break;
    }
  }
  return found;
}
