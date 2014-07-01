#include "Config.hpp"
#include "helpers.hpp"
#include "log.h"

int main(int argc , char ** argv)
{
  FILELog::ReportingLevel() = FILELog::FromString("DEBUG");

  Config conf;
  FILE_LOG(logINFO) << "Config is finished: " << btos(conf.read("init.cfg"));
}
