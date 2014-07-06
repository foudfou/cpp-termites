#ifndef _OPTIONPARSER_H_
#define _OPTIONPARSER_H_

#include <stdio.h>
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
    "  -o, --ouput		Log ouput\n"
    "  -H, --height		Board height\n"
    "  -W, --width		Board width\n"
    "  -t, --termites	Number of termites\n"
    "  -c, --chips		Number of wood chips\n"
    "  -T, --tics		Number of tics\n";

public:
  OptionParser(std::shared_ptr<Config> cnf);
  virtual ~OptionParser();

  bool parse(const int argc, char *const * argv);
  void processInOrder();
  bool check();
  FILE* getLogFile();
  void setLogFile(std::string filename);
  std::string getConfigFileName();

private:
  std::shared_ptr<Config> conf;
  std::map<std::string, std::string> options;
  std::string logFileName;
  FILE* logFile;
};

#endif /* _OPTIONPARSER_H_ */
