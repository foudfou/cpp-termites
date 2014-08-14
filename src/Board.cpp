#include <algorithm>
#include <stdexcept>
#include "ext/log.h"
#include "Board.hpp"
#include "Termite.hpp"

Board::Board() : width(0), height(0) {}

Board::~Board() { }

void Board::resize(tmt::Position outer)
{
  width = outer.col; height = outer.row;
  grid.resize(height*width, nullptr);
}

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

/*
  – Les termites se déplacent aléatoirement de case en case dans les huit
  directions possibles.
  – Si une termite sans brindille arrive près d’une brindille, elle la prend
  sur elle.
  – Si une termite avec brindille arrive près d’une brindille elle la dépose
  dans une case vide autour du copeau trouvé.
  – Deux termites ne peuvent se trouver sur une même case.
  – Une termite renonce à un déplacement si celui-ci doit l’amener en dehors
  des limites du terrain ou sur une case où se trouve déjà une autre termite.
*/
void Board::runTermite(const tmt::Position& pos)
{
  tmt::Position newPos = moveTermite(pos);
  toggleTermiteLoadMaybe(newPos);
}

tmt::Position Board::moveTermite(const tmt::Position& pos)
{
  auto dirs  = getAdjacentPositions(pos);
  std::uniform_int_distribution<int> dis(0, dirs.size()-1);
  int next = dis(tmt::random_gen());
  tmt::Position newPos = dirs[next];
  if (!(*this)(newPos))
  {
    (*this)(newPos) = (*this)(pos);
    (*this)(pos) = nullptr;
  }
  else
  {
    tmt::log(logDEBUG, _("Termite at (%d, %d) can't move to already occupied "
                        "(%d, %d)."),
             pos.col, pos.row, newPos.col, newPos.col);
    newPos = pos;
  }
  return newPos;
}

void Board::toggleTermiteLoadMaybe(const tmt::Position& newPos)
{
  std::shared_ptr<Termite> termite =
    std::dynamic_pointer_cast<Termite>((*this)(newPos));

  auto dirs = getAdjacentPositions(newPos);
  for (auto dir : dirs)
  {
    PiecePtr p = (*this)(dir);
    if (p && p->isWoodChip())
    {
      if (termite->isLoaded())
      {
        auto commonPositions = intersectAdjacentPositions(newPos, dir);
        tmt::Position dropPos; bool dropFound = false;
        for (auto p : commonPositions) {
          if (!(*this)(p))  // TODO: pick randomly
          {
            dropPos = p;
            dropFound = true;
            break;
          }
        }
        if (dropFound) {
          (*this)(dropPos) = termite->unload();
        }
      }
      else
      {
        termite->load(p);
        (*this)(dir) = nullptr;
      }
      break;
    }
  }
}

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
