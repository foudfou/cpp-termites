/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <libintl.h>
#include <locale.h>
#include <cstdlib>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Config.hpp"
#include "Options.hpp"
#include "Game.hpp"
#include "helpers.hpp"
#include "ext/log.h"
#include "config.h"

int main(int argc , char* argv[])
{
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE_NAME, LOCALEDIR);
  textdomain(PACKAGE_NAME);

  FILELog::ReportingLevel() = FILELog::FromString("INFO");

  std::shared_ptr<Config> conf(new Config);
  Options options;
  options.setConfig(conf);
  int res = options.parse(argc, argv);
  if (res == tmt::Options::NOOP)
    return EXIT_SUCCESS;
  if (res == tmt::Options::ERROR)
    return EXIT_FAILURE;

  auto confFileName = options.getConfigFileName();
  if (!confFileName.empty() && !conf->read(confFileName))
    return EXIT_FAILURE;

  Game game;
  game.init(conf);

  if (conf->getGraphicalMode())
  {
    const unsigned SPRITE_WIDTH  = 32;
    const unsigned SPRITE_HEIGHT = 32;

    unsigned width = conf->getWidth();
    unsigned height = conf->getHeight();

    sf::RenderWindow window(
      sf::VideoMode(SPRITE_WIDTH*width, SPRITE_HEIGHT*height),
      PACKAGE_NAME);
    window.setVerticalSyncEnabled(true);

    sf::Texture texture;
    if (!texture.loadFromFile("data/square-outline-32x32.png"))
      return EXIT_FAILURE;

    std::vector<sf::Sprite> grid(width*height);
    for (unsigned h=0; h<height; ++h) {
      for (unsigned c=0; c<width; ++c) {
        sf::Sprite sprite(texture);
        sprite.setPosition(SPRITE_WIDTH*c, SPRITE_HEIGHT*h);
        sprite.setColor(sf::Color(255, 255, 255, 200));
         // sprite.setTextureRect(sf::IntRect(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT));
        grid[width*h+c] = sprite;
      }
    }

    while (window.isOpen())
    {
      sf::Event event;
      while (window.pollEvent(event))
      {
        if (event.type == sf::Event::Closed)
          window.close();
        if (event.type == sf::Event::KeyPressed &&
            event.key.alt && event.key.code == sf::Keyboard::Q)
          window.close();
      }

      window.clear(sf::Color::Black);

      for (int i=0, len=grid.size(); i<len; ++i) {
        window.draw(grid[i]);
      }

      window.display();
    }

  }
  else
  {
    FILE_LOG(logINFO) << "BEFORE:" << std::endl << game.board.dump();
    while (game.tic()) ;
    FILE_LOG(logINFO) << "AFTER:" << std::endl << game.board.dump();
  }

  return EXIT_SUCCESS;
}
