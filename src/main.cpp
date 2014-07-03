#include <memory>
#include "Config.hpp"
#include "helpers.hpp"
#include "log.h"

int main(int argc , char ** argv)
{
  FILELog::ReportingLevel() = FILELog::FromString("DEBUG");

  std::shared_ptr<Config> conf(new Config);
  FILE_LOG(logINFO) << "Config parsing finished: " << btos(conf->read("init.cfg"));
}
