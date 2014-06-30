#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <functional>
#include <iostream>
#include <map>
#include <string>


class Config
{
public:
  Config();
  virtual ~Config();

  static const int FILE_SIZE_MAX = 2048;

  bool read(std::string const& configFile);

private:
  int time;
  int width;
  int height;
  std::map<std::string, int> chips;

  inline void reportErr(std::string cfgFile, int line) {
    std::cerr << "Wrong format: " << cfgFile << ":" << line << std::endl;
  }
};

#endif /* _CONFIG_H_ */
