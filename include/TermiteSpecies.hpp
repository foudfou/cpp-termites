#ifndef _TERMITESPECIES_H_
#define _TERMITESPECIES_H_

#include <memory>
#include <string>
#include <list>
#include "WoodSpecies.hpp"

// single species
class TermiteSpecies
{
public:
  TermiteSpecies(const std::string& spc, const std::list<WoodSpeciesPtr>& wd);
  virtual ~TermiteSpecies();

  std::string getName() const;
  bool likes(const WoodSpeciesPtr& woodSpecies) const;

private:
  std::string name;
  std::list<WoodSpeciesPtr> woodsLiked;
};

typedef std::shared_ptr<TermiteSpecies> TermiteSpeciesPtr;


#endif /* _TERMITESPECIES_H_ */
