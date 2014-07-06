/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <memory>
#include "Config.hpp"
#include "OptionParser.hpp"
#include "helpers.hpp"
#include "log.h"

int main(int argc , char ** argv)
{
  FILELog::ReportingLevel() = FILELog::FromString("INFO");

  std::shared_ptr<Config> conf(new Config);
  // FILE_LOG(logINFO) << "Config parsing finished: " << btos(conf->read("init.cfg"));

  OptionParser options = OptionParser(conf);
  options.parse(argc, argv);
}
