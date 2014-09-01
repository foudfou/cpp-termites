/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <algorithm>
#include <stdexcept>
#include "Board.hpp"
#include "Termite.hpp"

Board::Board() : width(0), height(0) {}

Board::~Board() { }

int Board::getWidth() const {return width;}

int Board::getHeight() const {return height;}

/** Resizes the board to `{width,height}` (the first outer cell). */
void Board::resize(tmt::Position outer)
{
  width = outer.col; height = outer.row;
  grid.resize(height*width, nullptr);
}

/** Computes a string representing the board. */
std::string Board::dump() const
{
  std::string brd = " ";
  const char* DIGITS = "0123456789";
  for (int i=0, len=width; i<len; i++) brd.push_back(DIGITS[i%10]);

  for (int i=0, len=grid.size(); i<len; ++i)
  {
    if (i%width == 0) {
      brd += '\n';
      brd += DIGITS[(i/width)%10];
    }
    auto pc = grid[i];
    if (pc)
    {
      if (pc->isTermite())
      {
        if (std::dynamic_pointer_cast<Termite>(pc)->isLoaded())
          brd += "O";
        else
          brd += "T";
      }
      else if (pc->isWoodChip())
        brd += "w";
      else
        throw std::logic_error("unknown piece type");
    }
    else
    {
      brd += "-";
    }
  }
  return brd;
}

/** Computes the list of termite positions. */
std::vector<tmt::Position> Board::getTermitePositions() const
{
  std::vector<tmt::Position> tpositions;
  for (int i=0, len=grid.size(); i<len; ++i)
  {
    PiecePtr piece = grid[i];
    if (piece && piece->isTermite())
      tpositions.push_back(tmt::rankToPosition(i, width));
  }
  return tpositions;
}

/** Computes the list of the positions adjacent to a given position, considering
 * board borders. */
std::vector<tmt::Position>
Board::getAdjacentPositions(const tmt::Position& pos) const
{
  std::vector<tmt::Position> positions;
  // we use pairs here, as Position is based on unsigned.
  std::vector< std::pair<int, int> > moves =
    {{-1,-1},{0,-1},{1,-1}, {-1,0},{1,0}, {-1,1},{0,1},{1,1}};
  for (int i=0, len=moves.size(); i<len; ++i)
  {
    int col = pos.col + moves[i].first, row = pos.row + moves[i].second;
    if (col >= 0 && col < width &&
        row >= 0 && row < height)
      positions.push_back(
        tmt::Position({static_cast<unsigned>(col), static_cast<unsigned>(row)}));
  }
  return positions;
}

/** Computes the list of the adjacent positions that two positions have in
 * common, considering board borders. */
std::vector<tmt::Position>
Board::intersectAdjacentPositions(const tmt::Position& pos1,
                                  const tmt::Position& pos2) const
{
  auto adj1 = getAdjacentPositions(pos1);
  auto adj2 = getAdjacentPositions(pos2);
  std::sort(adj1.begin(), adj1.end());
  std::sort(adj2.begin(), adj2.end());

  std::vector<tmt::Position> inter;
  std::set_intersection(adj1.begin(), adj1.end(),
                        adj2.begin(), adj2.end(),
                        std::back_inserter(inter));
  return inter;
}
