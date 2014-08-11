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

private:
  TermiteSpeciesPtr species;
};

#endif /* _TERMITE_H_ */
