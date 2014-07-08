/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <cstdlib>
#include <memory>
#include "Config.hpp"
#include "OptionParser.hpp"
#include "helpers.hpp"
#include "log.h"

int main(int argc , char ** argv)
{
  FILELog::ReportingLevel() = FILELog::FromString("INFO");

  std::shared_ptr<Config> conf(new Config);
  OptionParser options(conf);
  if (!options.parse(argc, argv))
    return EXIT_SUCCESS;

  auto confFileName = options.getConfigFileName();
  if (!confFileName.empty() && !conf->read(confFileName))
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
