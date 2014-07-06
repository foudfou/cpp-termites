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
  if (!confFileName.empty()) {
    FILE_LOG(logINFO) << "Config parsing finished: "
                      << tmt::btos(conf->read(confFileName));
  }

  if (auto logFile = options.getLogFile()) {
    fclose(logFile);
  }

  std::exit(EXIT_SUCCESS);
}
