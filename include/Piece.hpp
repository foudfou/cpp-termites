#ifndef _PIECE_H_
#define _PIECE_H_

#include <iostream>

class Piece
{
public:
  Piece();
  virtual ~Piece();

  virtual bool isWoodChip() const = 0;
  virtual bool isTermite() const = 0;

  virtual void print(std::ostream& out) const = 0;
};

#endif /* _PIECE_H_ */
