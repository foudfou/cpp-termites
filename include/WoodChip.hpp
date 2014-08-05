#ifndef _WOODCHIP_H_
#define _WOODCHIP_H_

#include "Piece.hpp"
#include "WoodSpecies.hpp"

class WoodChip : public Piece
{
  WoodSpecies* species;

public:
  WoodChip();
  virtual ~WoodChip();

  virtual bool isWoodChip() const;
  virtual void print(std::ostream& out) const;
};

#endif /* _WOODCHIP_H_ */
