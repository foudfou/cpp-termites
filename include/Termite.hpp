#ifndef _TERMITE_H_
#define _TERMITE_H_

#include "Piece.hpp"
#include "TermiteSpecies.hpp"

class Termite : public Piece
{
  TermiteSpecies* species;

public:
  Termite();
  virtual ~Termite();

  virtual bool isTermite() const;
  virtual void print(std::ostream& out) const;
};

#endif /* _TERMITE_H_ */
