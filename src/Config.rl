/* -*- mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
#include "Config.hpp"

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.hpp"
#include "log.h"

Config::Config(): word{0}, key{0} {}

Config::~Config() {}

%%{

  machine termites_conf;

  action error_any {
    FILE_LOG(logERROR) << "Parse error at line " << lineCount;
    fbreak;
  }

  action debug_any {
    FILE_LOG(logERROR) << "error line " << lineCount << " on char '" << fc \
                       << "' after '" << *(p-1) << "'";
  }

  action line_count_inc {
    ++lineCount;
  }

  action mark {
    mark = fpc;
  }

  action time_def {
    extractToken(word, fpc, mark);
    setTime(std::stoi(word));
  }

  # FIXME: is there no way to refactor *_def actions ?
  action width_def {
    extractToken(word, fpc, mark);
    setWidth(std::stoi(word));
  }

  action height_def {
    extractToken(word, fpc, mark);
    setHeight(std::stoi(word));
  }

  action list_init {
    list.clear();
  }

  action list_append {
    extractToken(word, fpc, mark);
    list.push_back(word);
  }

  action chips_def {
    extractToken(word, fpc, mark);
    setChips(listToMap(list));
  }

  action key {
    extractToken(key, fpc, mark);
  }

  action hash_init {
    hash.clear();
  }

  action hash_insert {
    auto lookup = hash.find(key);
    if (lookup == hash.end())
      hash[key] = listToMap(list);
    else
      FILE_LOG(logWARNING) << "Duplicate key ignored: " << key;
  }

  action species_def {
    if (!checkSpecies(hash)) fbreak;
    setSpecies(hash);
  }

  include termites_conf_core "rl/termites_conf.rl";

}%%

%% write data noerror nofinal;


void Config::setTime(int t)
{
  time = t;
  FILE_LOG(logDEBUG) << "time set to " << time;
}

void Config::setWidth(int t)
{
  width = t;
  FILE_LOG(logDEBUG) << "width set to " << width;
}

void Config::setHeight(int t)
{
  height = t;
  FILE_LOG(logDEBUG) << "height set to " << height;
}

// TODO: this parameter is useless, and redundant, as it can be derived from
// the species. we need to remove it in the future.
void Config::setChips(const std::map<std::string, int>& chps)
{
  chips = chps;
  if (FILELog::ReportingLevel() >= logDEBUG)
  {
    FILE_LOG(logDEBUG) << "CHIPS=|" << mapToString(chips);
  }
}

void Config::setSpecies(const std::map<std::string, std::map<std::string, int>>& spcs)
{
  species = spcs;
  for (auto sp: species) {
    FILE_LOG(logDEBUG) << "SPECIES=" << sp.first << " -> |" << mapToString(sp.second);
  }
}

bool Config::checkSpecies(const std::map<std::string, std::map<std::string, int>>& spcs)
{
  std::map<std::string, int> woods;
  for (auto sp: spcs)
    for (auto wood: sp.second)
      woods[wood.first]++;

  for (auto w: woods)
  {
    auto lookup = chips.find(w.first);
    if (lookup == chips.end()) {
      FILE_LOG(logERROR) << "Missing chip definition: " << w.first;
      return false;
    }
  }

  return true;
}

void Config::extractToken(TmpString& dest, const char*& cur, const char*& start)
{
  int len = cur - start;
  memcpy((void*)dest, (void*)start, len);
  dest[len] = '\0';
}


bool Config::read(std::string const& configFile) {
  /* We'll buffer the whole config file. We might need to parse by chunk. */
  if (fileSize(configFile) > FILE_SIZE_MAX)
  {
    FILE_LOG(logERROR) << "Config file too big.";
    exit(EXIT_FAILURE);
  }

  std::ifstream file;
  file.open(configFile);
  if (!file.good())
  {
    FILE_LOG(logERROR) << "Cannot read configuration file: " << configFile;
    return false;
  }

  std::stringstream buffer;
  std::string conf;
  buffer << file.rdbuf();
  conf = buffer.str();

  file.close();               // close file

  int cs = 0;
  const char *p, *pe;
  p = conf.c_str();             // pointer begin
  pe = p + strlen(p);           // pointer end
  char *eof = nullptr;

  int lineCount = 1;

  const char *mark = nullptr;
  std::list<std::string> list;
  std::map<std::string, std::map<std::string, int>> hash;

  %% write init;

  %% write exec;

  FILE_LOG(logINFO) << "Config has errors: " << btos(parserHasError(cs));

  return parserIsFinished(cs);
}

bool Config::parserHasError(int cs) {
  return cs == %%{ write error; }%%;
}

bool Config::parserIsFinished(int cs) {
  return cs >= %%{ write first_final; }%%;
}
