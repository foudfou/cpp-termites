#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>
#include <ostream>
#include "helpers.hpp"
#include "Piece.hpp"

// FIXME: Board should not be renamed to Game (because it comprises rules
// also): *World* should be renamed to Game, and most of Board's methods should
// be moved to Game.
class Board
{
public:
  Board();
  virtual ~Board();

  void resize(tmt::Position outer);
  std::vector<tmt::Position> getTermitePositions() const;
  void runTermite(const tmt::Position& pos);
  std::vector<tmt::Position> intersectAdjacentPositions(
    const tmt::Position& pos1, const tmt::Position& pos2) const; // XXX: should be private ?
  std::vector<tmt::Position> getAdjacentPositions(const tmt::Position& pos) const;
  std::string dump() const;

  /* we could make `grid` public, but this one's handy */
  inline PiecePtr& operator()(const tmt::Position pos) {
    return grid.at(width*pos.row+pos.col);
  }

private:
  int width;
  int height;
  std::vector<PiecePtr> grid;

  tmt::Position moveTermite(const tmt::Position& pos);
  void toggleTermiteLoadMaybe(const tmt::Position& newPos);
};

#endif /* _BOARD_H_ */
