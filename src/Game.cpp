#include <iostream>
#include "ext/log.h"
#include "helpers.hpp"
#include "Piece.hpp"
#include "Game.hpp"
#include "Termite.hpp"
#include "WoodChip.hpp"

Game::Game() : tics(0) {}

Game::~Game() {}

std::string Game::dumpWoodSpecies() const
{
  std::string str;
  for (auto& wspc : woodSpecies) {
    str += "|" + wspc->getName();
  }
  return str;
}

std::string Game::dumpTermiteSpecies() const
{
  std::string str;
  for (auto& tspc : termiteSpecies) {
    str += "|" + tspc->getName();
  }
  return str;
}

WoodSpeciesPtr Game::getWoodSpecies(const std::string& name) const
{
  return tmt::find(woodSpecies, name);
}

TermiteSpeciesPtr Game::getTermiteSpecies(const std::string& name) const
{
  return tmt::find(termiteSpecies, name);
}

void Game::init(std::shared_ptr<Config> conf)
{
  tics = conf->getTics();

  board.resize({conf->getWidth(), conf->getHeight()});
  for (auto& kv : conf->getChips()) {
    woodSpecies.push_back(WoodSpeciesPtr(new WoodSpecies(kv.first)));
  }

  for (auto& t2w : conf->getSpecies()) {
    std::list<WoodSpeciesPtr> wlist;
    for (auto& wspcName : t2w.second) {
      auto wspc = getWoodSpecies(wspcName.first);
      wlist.push_back(wspc);
    }
    TermiteSpeciesPtr tspc(new TermiteSpecies(t2w.first, wlist));
    termiteSpecies.push_back(tspc);
  }

  for (auto& ent : conf->getTermitePositions()) {
    TermiteSpeciesPtr tspc = getTermiteSpecies(ent.species);
    board({ent.pos.col, ent.pos.row}) = PiecePtr(new Termite(tspc));
  }

  for (auto& ent : conf->getChipPositions()) {
    WoodSpeciesPtr wspc = getWoodSpecies(ent.species);
    board({ent.pos.col, ent.pos.row}) = PiecePtr(new WoodChip(wspc));
  }
}

unsigned Game::tic()
{
  if (!tics)
    throw std::logic_error("No remaining tics");
  for (auto pos : board.getTermitePositions()) {
    runTermite(pos);
  }
  return --tics;
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
void Game::runTermite(const tmt::Position& pos)
{
  tmt::Position newPos = moveTermite(pos);
  toggleTermiteLoadMaybe(newPos);
}

tmt::Position Game::moveTermite(const tmt::Position& pos)
{
  auto dirs  = board.getAdjacentPositions(pos);
  std::uniform_int_distribution<int> dis(0, dirs.size()-1);
  int next = dis(tmt::random_gen());
  tmt::Position newPos = dirs[next];
  if (!board(newPos))
  {
    board(newPos) = board(pos);
    board(pos) = nullptr;
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

void Game::toggleTermiteLoadMaybe(const tmt::Position& newPos)
{
  std::shared_ptr<Termite> termite =
    std::dynamic_pointer_cast<Termite>(board(newPos));

  auto dirs = board.getAdjacentPositions(newPos);
  for (auto dir : dirs)
  {
    PiecePtr p = board(dir);
    if (p && p->isWoodChip())
    {
      if (termite->isLoaded())
      {
        auto commonPositions = board.intersectAdjacentPositions(newPos, dir);
        tmt::Position dropPos; bool dropFound = false;
        for (auto p : commonPositions) {
          if (!board(p))  // TODO: pick randomly
          {
            dropPos = p;
            dropFound = true;
            break;
          }
        }
        if (dropFound) {
          board(dropPos) = termite->unload();
        }
      }
      else
      {
        termite->load(p);
        board(dir) = nullptr;
      }
      break;
    }
  }
}
