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
  void populate(std::shared_ptr<Config> conf);

private:
  World();
  ~World();

  // copy not permitted
  World(const World&) = delete;
  void operator=(const World&) = delete;

  Board board;
  std::list<TermiteSpecies*> termiteSpecies;
  std::list<WoodSpecies*> woodSpecies;
};

#endif /* _WORLD_H_ */
