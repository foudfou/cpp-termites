#include "World.hpp"

World::World() { }

World::~World() { }

void World::populate(std::shared_ptr<Config> conf)
{
  board.resize({conf->getWidth(), conf->getHeight()});
  // Chips chips;
  // Species species;
  // Positions termitePositions;
  // Positions chipPositions;
}
