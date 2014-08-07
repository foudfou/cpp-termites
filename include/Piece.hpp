#ifndef _PIECE_H_
#define _PIECE_H_

#include <iostream>

class Piece
{
public:
  Piece();
  virtual ~Piece();

  virtual bool isWoodChip() const;
  virtual bool isTermite() const;

  virtual void print(std::ostream& out) const = 0;
};

#endif /* _PIECE_H_ */
