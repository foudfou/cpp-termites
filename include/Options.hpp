#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <cstdio>
#include <memory>
#include <string>
#include "Config.hpp"

class Options
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

  enum InitMode { FILE, OPTS };

public:
  Options();
  virtual ~Options();
  /* copy not allowed, as Options needs to manage a FILE* for FILELog */
  Options(const Options& that) = delete;
  Options& operator=(const Options& that) = delete;

  void setConfig(std::shared_ptr<Config> cnf);
  bool parse(const int argc, char* const* argv);
  std::string getConfigFileName();

private:
  std::shared_ptr<Config> conf;
  std::map<std::string, std::string> options;
  std::FILE* logFile;
  InitMode initMode;
  bool check();
  bool processInOrder();
  bool setLogFile(std::string filename);
  std::vector<Config::Entity>
  buildEntities(int amount, const std::vector<int> &randoms, int width,
                int offset) const;
};

#endif /* _OPTIONS_H_ */
