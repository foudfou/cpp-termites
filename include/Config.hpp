#ifndef _CONFIG_H_
#define _CONFIG_H_

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

  void setTime(int t);
  void setWidth(int t);
  void setHeight(int t);
  void setChips(const std::map<std::string, int>& chps);

private:
  int time;
  int width;
  int height;
  // we store wood chips into a map, as wood species are unique
  std::map<std::string, int> chips;

  bool parserHasError(int cs);
  bool parserIsFinished(int cs);
};

#endif /* _CONFIG_H_ */
