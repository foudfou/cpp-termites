#include "Board.hpp"

Board::Board() { }

Board::~Board() { }

Piece& Board::piece(const Position pos) const
{
}

void Board::moveTermite()
{
}

void Board::display(std::ostream& out) const
{
  out << "Board" << std::endl;
}
