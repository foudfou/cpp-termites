#include "World.hpp"

World::World() { }

World::~World() { }

void World::populate(std::shared_ptr<Config> conf)
{
  board.resize({conf->getHeight(), conf->getWidth()});
  // Chips chips;
  // Species species;
  // Positions termitePositions;
  // Positions chipPositions;
}
