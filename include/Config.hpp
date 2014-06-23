#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <functional>
#include <iostream>
#include <map>
#include <string>


class Config
{
  using ParseFunc = std::function<void(Config&, std::string const&)>;
  std::map<std::string, ParseFunc> parseHandler {
    {"temps",   std::mem_fn(&Config::timeDef)},
    {"largeur", std::mem_fn(&Config::widthDef)},
    {"hauteur", std::mem_fn(&Config::heightDef)},
    {"copeaux", std::mem_fn(&Config::chipsDef)},
  };

public:
  Config();
  virtual ~Config();

  bool read(std::string const& configFile);

private:
  int time;
  int width;
  int height;
  std::map<std::string, int> chips;

  void timeDef(std::string val);
  void widthDef(std::string val);
  void heightDef(std::string val);
  void chipsDef(std::string val);
  void parseConsume(const std::string& key, const std::string& val);

  inline void reportErr(std::string cfgFile, int line) {
    std::cerr << "Wrong format: " << cfgFile << ":" << line << std::endl;
  }
};

#endif /* _CONFIG_H_ */
