#ifndef _OPTIONPARSER_H_
#define _OPTIONPARSER_H_

#include <memory>
#include <string>
#include "Config.hpp"

class OptionParser
{
  const char * usage =
    "usage: termites [ options ] [ initfile ]\n\n"
    "Simulates the behaviour of termites among wood chips.\n\n"
    "  -h, --help\n"
    "  -d, --debug\n"
    "  -v, --version\n"
    "  -H, --height		Board height\n"
    "  -W, --width		Board width\n"
    "  -t, --termites	Number of termites\n"
    "  -c, --chips		Number of wood chips\n"
    "  -T, --tics		Number of tics\n";

public:
  OptionParser(std::shared_ptr<Config> cnf);
  virtual ~OptionParser();

  bool parse(const int argc, char *const * argv);
  void rand();

private:
  std::shared_ptr<Config> conf;

  int debug;
  std::string configFile;
};

#endif /* _OPTIONPARSER_H_ */
