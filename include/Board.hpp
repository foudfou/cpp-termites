#ifndef _BOARD_H_
#define _BOARD_H_

#include <string>
#include <vector>
#include "helpers.hpp"
#include "Piece.hpp"

/** A class representing a 2D finite board for the game.
 * Cells are localized by their Position. The `{0,0}` position is the origin.
 */
class Board
{
public:
  Board();
  virtual ~Board();

  int getWidth() const;
  int getHeight() const;
  void resize(tmt::Position outer);
  std::string dump() const;
  std::vector<tmt::Position> getTermitePositions() const;
  std::vector<tmt::Position> getAdjacentPositions(const tmt::Position& pos) const;
  std::vector<tmt::Position> intersectAdjacentPositions(
    const tmt::Position& pos1, const tmt::Position& pos2) const;

  /** Facility operator to get and set a cell content.
   *
   * @note We could make `grid` public, but this one's handy. */
  inline PiecePtr& operator()(const tmt::Position pos) {
    return grid.at(width*pos.row+pos.col);
  }

private:
  int width;
  int height;
  std::vector<PiecePtr> grid;
};

#endif /* _BOARD_H_ */
