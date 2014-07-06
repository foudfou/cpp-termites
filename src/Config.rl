/* -*- mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "Config.hpp"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "log.h"
#include "helpers.hpp"

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

  action tics_def {
    setTics(std::stoi(pstate.integer));
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
    setChips(tmt::listToMap(pstate.list));
    pstate.word.clear();
    pstate.list.clear();
  }

  action key {
    pstate.key.append(1, fc);
  }

  action hash_insert {
    auto lookup = pstate.hash.find(pstate.key);
    if (lookup == pstate.hash.end())
      pstate.hash[pstate.key] = tmt::listToMap(pstate.list);
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
    FILE_LOG(logDEBUG) << "pstate.key=" << pstate.word;
    storeEntityPosition(termitePositions, pstate.word, pstate.xcoord, pstate.ycoord);
    pstate.word.clear();
    pstate.xcoord.clear(); pstate.ycoord.clear();
  }

  action chip_pos {
    storeEntityPosition(chipPositions, pstate.word, pstate.xcoord, pstate.ycoord);
    pstate.word.clear();
    pstate.xcoord.clear(); pstate.ycoord.clear();
  }

  include termites_conf_core "rl/termites_conf.rl";

}%%

%% write data noerror nofinal;


int Config::getTics() {return tics;}

void Config::setTics(int t)
{
  tics = t;
  FILE_LOG(logDEBUG) << "tics set to " << tics;
}

int Config::getWidth() {return width;}

void Config::setWidth(int t)
{
  width = t;
  FILE_LOG(logDEBUG) << "width set to " << width;
}

int Config::getHeight() {return height;}

void Config::setHeight(int t)
{
  height = t;
  FILE_LOG(logDEBUG) << "height set to " << height;
}

// TODO(fb): this parameter is useless, and redundant, as it can be derived
// from the species. we need to remove it in the future.
void Config::setChips(const Chips &chps)
{
  chips = chps;
  if (FILELog::ReportingLevel() >= logDEBUG)
  {
    FILE_LOG(logDEBUG) << "CHIPS=|" << tmt::mapToString(chips);
  }
}

void Config::setSpecies(const Species &spcs)
{
  species = spcs;
  if (FILELog::ReportingLevel() >= logDEBUG) {
    for (auto sp : species) {
      FILE_LOG(logDEBUG) << "SPECIES=" << sp.first << " -> |"
                         << tmt::mapToString(sp.second);
    }
  }
}

bool Config::checkSpecies(const Species &spcs)
{
  Chips woods;
  for (auto sp : spcs)
    for (auto wood : sp.second)
      woods[wood.first]++;

  for (auto w : woods)
  {
    auto lookup = chips.find(w.first);
    if (lookup == chips.end()) {
      FILE_LOG(logERROR) << "Missing chip definition: " << w.first;
      return false;
    }
  }

  return true;
}

void Config::setTermitePositions(const std::vector<Entity> &tpos)
{
  termitePositions = tpos;
}

void Config::setChipPositions(const std::vector<Entity> &cpos)
{
  chipPositions = cpos;
}

void Config::storeEntityPosition(std::vector<Entity> &store,
                                 const TmpString &word,
                                 const TmpString &r, const TmpString &c)
{
  int row = std::stoi(r), col = std::stoi(c);
  Entity ent = {word, row, col};
  store.push_back(ent);
  std::string entity = "Unknown";
  if (&store == &termitePositions)
    entity = "termite";
  else if (&store == &chipPositions)
    entity = "chip";
  else
    FILE_LOG(logERROR) << "Unknown entity store.";
  FILE_LOG(logDEBUG) << entity << ": " << ent.species << " " << ent.row << ", " << ent.col;
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
    file.read(pstate.buffer, BUFFER_SIZE);
    parserExecute(pstate, pstate.buffer, file.gcount());
  }

  file.close();

  bool finished = parserFinish(pstate);
  bool checked = check();
  return finished && checked;
}

bool Config::check()
{
  return checkParamsDefined() && checkBounds();
}

bool Config::checkParamsDefined()
{
  struct param_check_t { std::string msg; int val; };
  param_check_t params[] = {
    { "Undefined parameter 'tics'.", tics },
    { "Undefined parameter 'height'.", height },
    { "Undefined parameter 'width'.", width },
    { "Undefined parameter 'chips'.", int(chips.size()) },
    { "Undefined parameter 'species'.", int(species.size()) },
    { "Missing termite positions.", int(termitePositions.size()) },
    { "Missing chip positions.", int(chipPositions.size()) },
  };
  for (auto p : params) {
    if (!p.val) {
      FILE_LOG(logERROR) << p.msg;
      return false;
    };
  }
  return true;
}

bool Config::checkBounds()
{
  struct member_t {
    std::vector<Entity>* positions;
    Species* spc;
    Chips* chp;
    std::string msg;
  };
  member_t members[] = {
    { &termitePositions, &species, nullptr, "species" },
    { &chipPositions, nullptr, &chips, "chips" },
  };

  for (auto mbr : members) {
    for (auto pos : *(mbr.positions)) {
      if ((mbr.spc && (*mbr.spc).find(pos.species) == (*mbr.spc).end()) ||
          (mbr.chp && (*mbr.chp).find(pos.species) == (*mbr.chp).end()))
      {
        FILE_LOG(logERROR) << "'" << pos.species
                           << "' (at " << pos.row << "," << pos.col
                           << ") is not defined in global species";
        return false;
      }
    }
  }
  return true;
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
  bool err = parserHasError(pstate), fin = parserIsFinished(pstate);
  FILE_LOG(logDEBUG) << "Config has errors: " << tmt::btos(err);
  FILE_LOG(logDEBUG) << "Config parsed to the end: " << tmt::btos(fin);
  return !err && fin;
}

bool Config::parserHasError(ParserState &pstate) {
  return pstate.fail || pstate.cs == %%{ write error; }%%;
}

bool Config::parserIsFinished(ParserState &pstate) {
  return !pstate.fail && pstate.cs >= %%{ write first_final; }%%;
}
