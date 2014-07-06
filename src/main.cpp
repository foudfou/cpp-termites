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
  OptionParser options = OptionParser(conf);
  if (!options.parse(argc, argv))
    std::exit(EXIT_FAILURE);

  auto confFileName = options.getConfigFileName();
  if (!confFileName.empty() && !conf->read(confFileName))
    std::exit(EXIT_FAILURE);

  if (auto logFile = options.getLogFile()) {
    fclose(logFile);
  }

  std::exit(EXIT_SUCCESS);
}
