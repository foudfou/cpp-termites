#include <stdexcept>
#include "Board.hpp"

Board::Board() : width(0), height(0) {}

Board::~Board() { }

void Board::resize(tmt::Position outer)
{
  width = outer.col; height = outer.row;
  grid.resize(height*width, nullptr);
}

void Board::moveTermite()
{
}

std::string Board::dump() const
{
  std::string brd;
  for (int i=0, len=grid.size(); i<len; ++i) {
    if (i%width == 0 && i>=width) brd += "\n";
    auto pc = grid[i];
    if (pc)
    {
      if (pc->isTermite())
        brd += "T";
      else if (pc->isWoodChip())
        brd += "W";
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
