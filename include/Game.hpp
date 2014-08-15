#ifndef _GAME_H_
#define _GAME_H_

#include <list>
#include <memory>
#include <string>
#include "helpers.hpp"
#include "Board.hpp"
#include "Config.hpp"
#include "TermiteSpecies.hpp"
#include "WoodSpecies.hpp"

class Game
{
public:
  Game();
  ~Game();

  Board board;

  WoodSpeciesPtr    getWoodSpecies(const std::string& name) const;
  inline size_t     getWoodSpeciesSize() const {return woodSpecies.size();}
  std::string       dumpWoodSpecies() const;
  TermiteSpeciesPtr getTermiteSpecies(const std::string& name) const;
  inline size_t     getTermiteSpeciesSize() const {return termiteSpecies.size();}
  std::string       dumpTermiteSpecies() const;
  void init(std::shared_ptr<Config> conf);
  unsigned tic();
  void runTermite(const tmt::Position& pos);

private:
  unsigned tics;
  std::list<WoodSpeciesPtr> woodSpecies;
  std::list<TermiteSpeciesPtr> termiteSpecies;

  tmt::Position moveTermite(const tmt::Position& pos);
  void toggleTermiteLoadMaybe(const tmt::Position& newPos);
};

#endif /* _GAME_H_ */
