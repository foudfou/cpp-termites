#include "Config.hpp"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "helpers.hpp"
#include "log.h"


Config::Config() {
}

Config::~Config() {}


/*
############################################################################
# Les lignes commençant par le caractère ’#’ sont des commentaires .
# Ce format , permet une lecture / écriture très simple via les classes
# std :: fstream .
############################################################################
temps : 2000
largeur : 80
hauteur : 50
copeaux : ( boulot chene baobab )
termites : rouge ( boulot ) verte ( boulot chene ) noir ( baobab boulot ) 10
termite : rouge 10 5
termite : rouge 23 34
copeau : boulot 42 23
copeau : baobab 1 3
 */

bool Config::read(std::string const& configFile) {
  std::ifstream file(configFile.c_str());

  if (!file.good())
  {
    std::cerr << "Cannot read configuration file: " << configFile << std::endl;
    return false;
  }

  int lineNb = 0;
  while (file.good() && !file.eof())
  {
    std::string line, key, val;
    std::string::size_type posColon;
    getline(file, line); lineNb++;
    line = trim(line);

    if (!line.length()) continue;
    if (line[0] == '#') continue;

    posColon = line.find(':');
    if (posColon == std::string::npos)
    {
      std::cerr << "Wrong format: " << configFile << ":" << lineNb << std::endl;
      return false;
    }
    key = trim(line.substr(0,posColon));
    val = trim(line.substr(posColon+1));
    parseConsume(key, val);
  }

}

void Config::timeDef(std::string val)
{
  FILE_LOG(logDEBUG) << "TIME=" << val;
  setTime(atoi(val.c_str()));
}

void Config::widthDef(std::string val)
{
  FILE_LOG(logDEBUG) << "WIDTH=" << val;
  setWidth(atoi(val.c_str()));
}

void Config::heightDef(std::string val)
{
  FILE_LOG(logDEBUG) << "HEIGHT=" << val;
  setHeight(atoi(val.c_str()));
}

void Config::parseConsume(const std::string& key, const std::string& val)
{
  if (key == "temps") timeDef(val);
  else if (key == "largeur") widthDef(val);
  else if (key == "hauteur") heightDef(val);
  else
  {
    std::cerr << "Unknown key: " << key << std::endl;
    return;
  }
}
