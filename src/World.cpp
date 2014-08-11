#include <algorithm>
#include <iostream>
#include "World.hpp"
#include "Termite.hpp"
#include "WoodChip.hpp"
#include "helpers.hpp"

World::World() { }

World::~World() { }

std::string World::dumpWoodSpecies() const
{
  std::string str;
  for (auto& wspc : woodSpecies) {
    str += "|" + wspc->getName();
  }
  return str;
}

std::string World::dumpTermiteSpecies() const
{
  std::string str;
  for (auto& tspc : termiteSpecies) {
    str += "|" + tspc->getName();
  }
  return str;
}

WoodSpeciesPtr World::getWoodSpecies(const std::string& name) const
{
  return tmt::find(woodSpecies, name);
}

TermiteSpeciesPtr World::getTermiteSpecies(const std::string& name) const
{
  return tmt::find(termiteSpecies, name);
}

void World::populate(std::shared_ptr<Config> conf)
{
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
    board({ent.pos.col, ent.pos.row}) = Board::PiecePtr(new Termite(tspc));
  }

  for (auto& ent : conf->getChipPositions()) {
    WoodSpeciesPtr wspc = getWoodSpecies(ent.species);
    board({ent.pos.col, ent.pos.row}) = Board::PiecePtr(new WoodChip(wspc));
  }
}
