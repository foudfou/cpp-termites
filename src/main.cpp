/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <libintl.h>
#include <locale.h>
#include <cstdlib>
#include <memory>
#include <vector>
#include "Config.hpp"
#include "Options.hpp"
#include "Game.hpp"
#include "Graphical.hpp"
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

  std::shared_ptr<Game> game(new Game);
  game->init(conf);

  if (conf->getGraphicalMode())
  {
    Graphical graphics(game);
    if (!graphics.run())
      return EXIT_FAILURE;
  }
  else
  {
    FILE_LOG(logINFO) << "BEFORE:" << std::endl << game->board.dump();
    while (game->tic()) ;
    FILE_LOG(logINFO) << "AFTER:" << std::endl << game->board.dump();
  }

  return EXIT_SUCCESS;
}
