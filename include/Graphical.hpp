#ifndef GRAPHICAL_H
#define GRAPHICAL_H

#include <memory>
#include "Game.hpp"

class Graphical
{
public:
  Graphical(std::shared_ptr<Game> gam);
  virtual ~Graphical();

  bool run();

private:
  std::shared_ptr<Game> game;
};

#endif /* GRAPHICAL_H */
