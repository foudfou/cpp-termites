/* -*- mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "helpers.hpp"
#include "Config.hpp"

Config::Config() {}

Config::~Config() {}

%%{

  machine termites_conf;

  action error_any {
    FILE_LOG(logERROR) << "Parse error at line " << pstate.lineCount;
    fbreak;
  }

  action debug_any {
    FILE_LOG(logERROR) << "error line " << pstate.lineCount << " on char '" \
                       << fc << "' after '" << *(p-1) << "'";
  }

  action line_count_inc {
    ++pstate.lineCount;
  }

  action integer {
    pstate.integer.append(1, fc);
  }

  action time_def {
    setTime(std::stoi(pstate.integer));
    pstate.integer.clear();
  }

  action width_def {
    setWidth(std::stoi(pstate.integer));
    pstate.integer.clear();
  }

  action height_def {
    setHeight(std::stoi(pstate.integer));
    pstate.integer.clear();
  }

  action word {
    pstate.word.append(1, fc);
  }

  action list_append {
    pstate.list.push_back(pstate.word);
    pstate.word.clear();
  }

  action chips_def {
    setChips(listToMap(pstate.list));
    pstate.word.clear();
    pstate.list.clear();
  }

  action key {
    pstate.key.append(1, fc);
  }

  action hash_insert {
    auto lookup = pstate.hash.find(pstate.key);
    if (lookup == pstate.hash.end())
      pstate.hash[pstate.key] = listToMap(pstate.list);
    else
      FILE_LOG(logWARNING) << "Duplicate key ignored: " << pstate.key;
    pstate.key.clear();
    pstate.list.clear();
  }

  action species_def {
    if (!checkSpecies(pstate.hash)) {
      pstate.fail = true;
      fbreak;
    }
    setSpecies(pstate.hash);
    pstate.hash.clear();
  }

  action xcoord {
    pstate.xcoord.append(1, fc);
  }

  action ycoord {
    pstate.ycoord.append(1, fc);
  }

  action termite_pos {
    storeEntityPos(termitePositions, pstate.key, pstate.xcoord, pstate.ycoord);
    pstate.word.clear();
    pstate.xcoord.clear(); pstate.ycoord.clear();
  }

  action chip_pos {
    storeEntityPos(chipPositions, pstate.key, pstate.xcoord, pstate.ycoord);
    pstate.word.clear();
    pstate.xcoord.clear(); pstate.ycoord.clear();
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
  if (FILELog::ReportingLevel() >= logDEBUG)
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

void Config::storeEntityPos(std::vector<Entity> &store, const TmpString &key,
                            const TmpString &x, const TmpString &y)
{
  Entity ent = {key, std::stoi(x), std::stoi(y)};
  store.push_back(ent);
  std::string entity = "Unknown";
  if (&store == &termitePositions)
    entity = "termite";
  else if (&store == &chipPositions)
    entity = "chip";
  else
    FILE_LOG(logERROR) << "Unknown entity store.";
  FILE_LOG(logDEBUG) << entity << ": " << ent.x << ", " << ent.y;
}

bool Config::read(std::string const& configFile)
{
  std::ifstream file;
  file.open(configFile);
  if (!file.good())
  {
    FILE_LOG(logERROR) << "Cannot read configuration file: " << configFile;
    return false;
  }

  ParserState pstate;
  parserInit(pstate);
  while (!file.eof())
  {
    file.read((char *)pstate.buffer, BUFFER_SIZE); // tellg() for position
    parserExecute(pstate, pstate.buffer, file.gcount());
  }

  file.close();

  return parserFinish(pstate);
}

void Config::parserInit(ParserState &pstate)
{
  pstate.cs = 0;
  pstate.lineCount = 1;
  pstate.fail = false;

  /* Access all ragel state variables via structure. */
  %% access pstate.;

  %% write init;
}

void Config::parserExecute(ParserState &pstate, const char *data, int len)
{
  pstate.eof = nullptr;

  pstate.p = data;
  pstate.pe = pstate.p + len;

  /* Access all ragel state variables via structure. */
  %% access pstate.;
  %% variable p pstate.p;
  %% variable pe pstate.pe;
  %% variable eof pstate.eof;

  %% write exec;
}

bool Config::parserFinish(ParserState &pstate)
{
  FILE_LOG(logINFO) << "Config has errors: " << btos(parserHasError(pstate));
  return parserIsFinished(pstate);
}

bool Config::parserHasError(ParserState &pstate) {
  return pstate.fail || pstate.cs == %%{ write error; }%%;
}

bool Config::parserIsFinished(ParserState &pstate) {
  return !pstate.fail && pstate.cs >= %%{ write first_final; }%%;
}
