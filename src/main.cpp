/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <memory>
#include "Config.hpp"
#include "helpers.hpp"
#include "log.h"

int main()
{
  FILELog::ReportingLevel() = FILELog::FromString("DEBUG");

  std::shared_ptr<Config> conf(new Config);
  FILE_LOG(logINFO) << "Config parsing finished: " << btos(conf->read("init.cfg"));
}
