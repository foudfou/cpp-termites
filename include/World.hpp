#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>
#include <memory>
#include <string>
#include "Board.hpp"
#include "Config.hpp"
#include "TermiteSpecies.hpp"
#include "WoodSpecies.hpp"

class World
{
public:
  static World& getInstance() {
    static World W;
    return W;
  }

  Board board;

  WoodSpeciesPtr getWoodSpecies(const std::string& name) const;
  inline size_t getWoodSpeciesSize() const {return woodSpecies.size();}
  std::string dumpWoodSpecies() const;
  TermiteSpeciesPtr getTermiteSpecies(const std::string& name) const;
  inline size_t getTermiteSpeciesSize() const {return termiteSpecies.size();}
  std::string dumpTermiteSpecies() const;
  void populate(std::shared_ptr<Config> conf);
  unsigned tic();

private:
  World();
  ~World();

  // copy not permitted
  World(const World&) = delete;
  void operator=(const World&) = delete;

  unsigned tics;
  std::list<WoodSpeciesPtr> woodSpecies;
  std::list<TermiteSpeciesPtr> termiteSpecies;
};

#endif /* _WORLD_H_ */
