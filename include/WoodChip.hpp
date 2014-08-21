#ifndef _WOODCHIP_H_
#define _WOODCHIP_H_

#include "Piece.hpp"
#include "WoodSpecies.hpp"

class WoodChip : public Piece
{
public:
  WoodChip(const WoodSpeciesPtr& spc);
  virtual ~WoodChip();

  virtual bool isWoodChip() const;
  std::string dump() const;
  WoodSpeciesPtr getSpecies() const;

private:
  WoodSpeciesPtr species;
};

#endif /* _WOODCHIP_H_ */
