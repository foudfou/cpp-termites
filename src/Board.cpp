#include <stdexcept>
#include "Board.hpp"

Board::Board() {}

Board::~Board() { }

void Board::resize(tmt::Position outer)
{
  width = outer.col; height = outer.row;
  grid.resize(height*width, nullptr);
}

void Board::moveTermite()
{
}

void Board::display(std::ostream& out) const
{
  out << "Board" << std::endl;
}
