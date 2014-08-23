#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "config.h"
#include "Graphical.hpp"
#include "Piece.hpp"
#include "Termite.hpp"

Graphical::Graphical(std::shared_ptr<Game> gam) : game(gam) {}

Graphical::~Graphical() {}

bool Graphical::run()
{
  unsigned width = game->board.getWidth();
  unsigned height = game->board.getHeight();

  sf::Texture texture;
  if (!texture.loadFromFile(std::string(DATA_DIR) + "/square-outline-32x32.png"))
    return false;

  sf::Vector2u textureSize = texture.getSize();
  const unsigned SPRITE_WIDTH  = textureSize.x;
  const unsigned SPRITE_HEIGHT = textureSize.y;

  sf::RenderWindow window(
    sf::VideoMode(SPRITE_WIDTH*width, SPRITE_HEIGHT*height),
    PACKAGE_NAME);
  window.setVerticalSyncEnabled(true);

  unsigned next = 1;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Q)
        window.close();
    }

    if (next)
      next = game->tic();

    window.clear(sf::Color::Black);

    for (unsigned h=0; h<height; ++h) {
      for (unsigned c=0; c<width; ++c) {
        sf::Sprite sprite(texture);
        sprite.setPosition(SPRITE_WIDTH*c, SPRITE_HEIGHT*h);
        PiecePtr piece = game->board({c,h});
        if (piece && piece->isTermite())
        {
          std::shared_ptr<Termite> termite =
            std::dynamic_pointer_cast<Termite>(piece);
          if (termite->isLoaded())
            sprite.setColor(sf::Color::Blue); // sf::Color(0, 0, 255, 200)
          else
            sprite.setColor(sf::Color::Red);
        }
        else if (piece && piece->isWoodChip())
        {
          sprite.setColor(sf::Color::Green);
        }
        else
        {
          sprite.setColor(sf::Color::Black);
        }

        window.draw(sprite);
      }
    }

    window.display();

  }
  return true;
}
