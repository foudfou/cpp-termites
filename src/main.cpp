#include "Config.hpp"
#include "log.h"

int main(int argc , char ** argv)
{
  FILELog::ReportingLevel() = FILELog::FromString("DEBUG");

  Config conf;
  conf.read("init.cfg");
}
