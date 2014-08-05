#ifndef _TERMITESPECIES_H_
#define _TERMITESPECIES_H_

#include <string>
#include <list>
#include "WoodSpecies.hpp"

// single species
class TermiteSpecies
{
  std::string species;
  std::list<WoodSpecies*> woodsLiked;

public:
  TermiteSpecies();
  virtual ~TermiteSpecies();

  bool likesWood(std::string wood);
};

#endif /* _TERMITESPECIES_H_ */
