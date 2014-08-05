#ifndef _WORLD_H_
#define _WORLD_H_

#include <string>
#include <list>
#include "TermiteSpecies.hpp"
#include "WoodSpecies.hpp"
#include "Board.hpp"

class World
{
public:
  static World& getInstance() {
    static World W;
    return W;
  }

  // copy not permitted
  World(const World&) = delete;
  void operator=(const World&) = delete;

private:
  World();
  ~World();

  std::list<TermiteSpecies*> termiteSpecies;
  std::list<WoodSpecies*> woodSpecies;
  Board board;
};

#endif /* _WORLD_H_ */
