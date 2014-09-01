/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#ifndef _WOODSPECIES_H_
#define _WOODSPECIES_H_

#include <memory>
#include <string>

class WoodSpecies
{
public:
  WoodSpecies(const std::string& nam);
  ~WoodSpecies();

  std::string getName() const;

private:
  std::string name;
};

typedef std::shared_ptr<WoodSpecies> WoodSpeciesPtr;

#endif /* _WOODSPECIES_H_ */
