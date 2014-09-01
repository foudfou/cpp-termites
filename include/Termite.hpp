/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#ifndef _TERMITE_H_
#define _TERMITE_H_

#include "Piece.hpp"
#include "TermiteSpecies.hpp"

class Termite : public Piece
{
public:
  Termite(const TermiteSpeciesPtr& spc);
  virtual ~Termite();

  virtual bool isTermite() const;
  virtual std::string dump() const;
  TermiteSpeciesPtr getSpecies() const;
  bool     isLoaded() const;
  void     load(const PiecePtr& chip);
  PiecePtr unload();

private:
  TermiteSpeciesPtr species;
  PiecePtr woodChip;
};

#endif /* _TERMITE_H_ */
