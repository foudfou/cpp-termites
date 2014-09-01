#include <cstdlib>
#include <stdexcept>
#include "config.h"
#include "helpers.hpp"
#include "Graphical.hpp"
#include "Piece.hpp"
#include "Termite.hpp"
#include "WoodChip.hpp"

Graphical::Graphical(std::shared_ptr<Game> gam)
  : game(gam), width(0), height(0), spriteWidth(0), spriteHeight(0)
{
  width = game->board.getWidth();
  height = game->board.getHeight();

  if (!texture.loadFromFile(std::string(DATA_DIR) + "/square-outline-32x32.png"))
    throw std::runtime_error("Can't load texture.");

  sf::Vector2u textureSize = texture.getSize();
  spriteWidth  = textureSize.x;
  spriteHeight = textureSize.y;

  window = std::shared_ptr<sf::RenderWindow>(
    new sf::RenderWindow(sf::VideoMode(spriteWidth*width, spriteHeight*height),
                         PACKAGE_NAME));
  window->setVerticalSyncEnabled(true);

  initColors();
}

Graphical::~Graphical() {}

bool Graphical::run()
{
  unsigned next = 1;
  while (window->isOpen())
  {
    sf::Event event;
    while (window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window->close();
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Q)
        window->close();
    }

    if (next)
      next = game->tic();

    window->clear(sf::Color::Black);

    update();

    window->display();

  }
  return true;
}

void Graphical::update()
{

  for (unsigned h=0; h<height; ++h) {
    for (unsigned c=0; c<width; ++c) {
      sf::Sprite sprite(texture);
      sprite.setPosition(spriteWidth*c, spriteHeight*h);
      PiecePtr piece = game->board({c,h});
      if (piece && piece->isTermite())
      {
        std::shared_ptr<Termite> termite =
          std::dynamic_pointer_cast<Termite>(piece);
        if (termite->isLoaded())
          sprite.setColor(termiteColors[termite->getSpecies()].loaded);
        else
          sprite.setColor(termiteColors[termite->getSpecies()].unloaded);
      }
      else if (piece && piece->isWoodChip())
      {
        std::shared_ptr<WoodChip> chip =
          std::dynamic_pointer_cast<WoodChip>(piece);
        sprite.setColor(woodColors[chip->getSpecies()]);
      }
      else
      {
        sprite.setColor(sf::Color::Black);
      }

      window->draw(sprite);
    }
  }
}

void Graphical::initColors()
{
  static const int COLOR_LO = 127;
  static const int COLOR_HI = 255;

  auto woodSpecies = game->getWoodSpeciesList();
  std::vector<int> intColors =
    tmt::splitRange(COLOR_LO, COLOR_HI, woodSpecies.size());
  auto it = intColors.begin();
  for (auto wspc : woodSpecies) {
    woodColors[wspc] =  sf::Color(0, *(it++), 0);
  }
  auto termiteSpecies = game->getTermiteSpeciesList();
  intColors = tmt::splitRange(COLOR_LO, COLOR_HI, termiteSpecies.size());
  it = intColors.begin();
  for (auto tspc : termiteSpecies) {
    termiteColors[tspc] = { sf::Color(0, 0, *(it)), sf::Color(*(it++), 0, 0) };
  }
}
