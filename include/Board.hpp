#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>
#include <ostream>
#include "Piece.hpp"


class Board
{
  std::vector<Piece*> grid;

public:
  struct Position {int x; int y;};

  Board();
  virtual ~Board();

  Piece& piece(const Position pos) const;
  void moveTermite();
  void display(std::ostream& out) const;
};

#endif /* _BOARD_H_ */
