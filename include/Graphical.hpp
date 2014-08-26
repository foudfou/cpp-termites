#ifndef GRAPHICAL_H
#define GRAPHICAL_H

#include <map>
#include <memory>
#include <utility>
#include <SFML/Graphics.hpp>
#include "Game.hpp"

class Graphical
{
public:
  Graphical(std::shared_ptr<Game> gam);
  ~Graphical();

  bool init();
  bool run();

private:
  std::shared_ptr<Game> game;

  bool initialized;
  unsigned width;
  unsigned height;

  std::shared_ptr<sf::RenderWindow> window;
  sf::Texture texture;
  unsigned spriteWidth;
  unsigned spriteHeight;

  std::map<WoodSpeciesPtr, sf::Color>    woodColors;
  std::map<TermiteSpeciesPtr, std::pair<sf::Color, sf::Color>> termiteColors;

  void update();
  void initColors();
};

#endif /* GRAPHICAL_H */
