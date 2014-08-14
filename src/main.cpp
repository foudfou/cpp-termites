/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <libintl.h>
#include <locale.h>
#include <cstdlib>
#include <memory>
#include "Config.hpp"
#include "Options.hpp"
#include "World.hpp"
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
  if (!options.parse(argc, argv))
    return EXIT_SUCCESS;
  // FIXME: we need to EXIT_SUCCESS with --help

  auto confFileName = options.getConfigFileName();
  if (!confFileName.empty() && !conf->read(confFileName))
    return EXIT_FAILURE;

  World& world = World::getInstance();
  world.populate(conf);

  FILE_LOG(logINFO) << "BEFORE:" << std::endl << world.board.dump();
  while (world.tic()) ;
  FILE_LOG(logINFO) << "AFTER:" << std::endl << world.board.dump();

  return EXIT_SUCCESS;
}
