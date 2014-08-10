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
  std::string dump() const;
};

#endif /* _WOODCHIP_H_ */
