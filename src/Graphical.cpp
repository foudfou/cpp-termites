#include <cstdlib>
#include "config.h"
#include "helpers.hpp"
#include "Graphical.hpp"
#include "Piece.hpp"
#include "Termite.hpp"
#include "WoodChip.hpp"

Graphical::Graphical(std::shared_ptr<Game> gam)
  : game(gam), initialized(false), width(0), height(0),
    spriteWidth(0), spriteHeight(0)
{}

Graphical::~Graphical() {}

// FIXME: could be done in ctor, but what if ctor fails ?
bool Graphical::init()
{
  if (initialized)
  {
    tmt::log(logERROR, _("Graphical already initialized."));
    return true;
  }

  width = game->board.getWidth();
  height = game->board.getHeight();

  if (!texture.loadFromFile(std::string(DATA_DIR) + "/square-outline-32x32.png"))
    return false;

  sf::Vector2u textureSize = texture.getSize();
  spriteWidth  = textureSize.x;
  spriteHeight = textureSize.y;

  window = std::shared_ptr<sf::RenderWindow>(
    new sf::RenderWindow(sf::VideoMode(spriteWidth*width, spriteHeight*height),
                         PACKAGE_NAME));
  window->setVerticalSyncEnabled(true);

  initColors();

  initialized = true;
  return true;
}

bool Graphical::run()
{
  if (!initialized)
  {
    tmt::log(logERROR, _("Graphical must be initialized before running."));
    return false;
  }
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
          sprite.setColor(termiteColors[termite->getSpecies()].first);
        else
          sprite.setColor(termiteColors[termite->getSpecies()].second);
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
  auto woodSpecies = game->getWoodSpeciesList();
  std::vector<int> intColors = tmt::pickn(woodSpecies.size(), 255);
  auto it = intColors.begin();
  for (auto wspc : woodSpecies) {
    woodColors[wspc] =  sf::Color(0, *(it++), 0);
  }
  auto termiteSpecies = game->getTermiteSpeciesList();
  intColors = tmt::pickn(woodSpecies.size(), 255);
  it = intColors.begin();
  for (auto tspc : termiteSpecies) {
    // FIXME: named tuples ?
    termiteColors[tspc] = { sf::Color(0, 0, *(it)), sf::Color(*(it++), 0, 0) };
  }
}
