#ifndef _OPTIONPARSER_H_
#define _OPTIONPARSER_H_

#include <cstdio>
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
  OptionParser();
  virtual ~OptionParser();
  /* copy not allowed, as OptionParser needs to manage a FILE* for FILELog */
  OptionParser(const OptionParser& that) = delete;
  OptionParser& operator=(const OptionParser& that) = delete;

  bool setConfig(std::shared_ptr<Config> cnf);
  bool parse(const int argc, char *const * argv);
  std::string getConfigFileName();

private:
  std::shared_ptr<Config> conf;
  std::map<std::string, std::string> options;
  std::FILE* logFile;

  bool check();
  bool processInOrder();
  bool setLogFile(std::string filename);
  std::vector<Config::Entity>
  buildEntities(int amount, const std::vector<int> &randoms, int width,
                int offset) const;
};

#endif /* _OPTIONPARSER_H_ */
