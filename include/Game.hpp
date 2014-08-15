#ifndef _GAME_H_
#define _GAME_H_

#include <list>
#include <memory>
#include <string>
#include "Board.hpp"
#include "Config.hpp"
#include "TermiteSpecies.hpp"
#include "WoodSpecies.hpp"

class Game
{
public:
  static Game& getInstance() {
    static Game W;
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
  Game();
  ~Game();

  // copy not permitted
  Game(const Game&) = delete;
  void operator=(const Game&) = delete;

  unsigned tics;
  std::list<WoodSpeciesPtr> woodSpecies;
  std::list<TermiteSpeciesPtr> termiteSpecies;
};

#endif /* _GAME_H_ */
