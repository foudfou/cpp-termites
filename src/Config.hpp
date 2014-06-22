#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <map>
#include <string>


class Config
{
  typedef void (Config::* ParseFunc)(std::string);
  typedef std::map<std::string, ParseFunc> ParseMap;

public:
  Config();
  virtual ~Config();

  bool read(std::string const& configFile);
  void setTime(int t) {Config::time = t;}
  void setWidth(int t) {Config::width = t;}
  void setHeight(int t) {Config::height = t;}

private:
  int time;
  int width;
  int height;

  ParseMap parseHandler;
  void timeDef(std::string val);
  void widthDef(std::string val);
  void heightDef(std::string val);
  void parseConsume(const std::string& key, const std::string& val);
};

#endif /* _CONFIG_H_ */
