#ifndef _BOARD_H_
#define _BOARD_H_

#include <memory>
#include <string>
#include <vector>
#include <ostream>
#include "helpers.hpp"
#include "Piece.hpp"

class Board
{
public:
  typedef std::shared_ptr<Piece> PiecePtr; // XXX: unique_ptr maybe ?

  Board();
  virtual ~Board();

  void resize(tmt::Position outer);
  void moveTermite();
  void display(std::ostream& out) const;

  /* we could make `grid` public, but this one's handy */
  inline PiecePtr& operator()(const tmt::Position pos) {
    return grid.at(width*pos.row+pos.col);
  }

private:
  int width;
  int height;
  std::vector<PiecePtr> grid;
};

#endif /* _BOARD_H_ */
