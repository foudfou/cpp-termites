/* -*- mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "Config.hpp"
#include <assert.h>
#include <fstream>
#include "ext/log.h"
#include "helpers.hpp"

Config::Config(): initialized(false), tics(0), width(0), height(0) {}

Config::~Config() {}

bool Config::Entity::operator==(const Entity& rhs) const
{
  return species == rhs.species &&
    row == rhs.row &&
    col == rhs.col;
}

%%{

  machine termites_conf;

  action error_any {
    tmt::log(logERROR, _("Parse error at line %d."), pstate.lineCount);
    fbreak;
  }

  action debug_any {
    tmt::log(logERROR, _("Error line %d on char '%c' after '%c'."),
                         pstate.lineCount, fc, *(p-1));
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
    setChips(tmt::listStringToMapStringInt(pstate.list));
    pstate.word.clear();
    pstate.list.clear();
  }

  action key {
    pstate.key.append(1, fc);
  }

  action hash_insert {
    auto lookup = pstate.hash.find(pstate.key);
    if (lookup == pstate.hash.end())
      pstate.hash[pstate.key] = tmt::listStringToMapStringInt(pstate.list);
    else
      tmt::log(logWARNING, _("Duplicate key ignored: %s"), pstate.key.c_str());
      pstate.key.clear();
    pstate.list.clear();
  }

  action species_def {
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
    tmt::log(logDEBUG, _("pstate.key=%s"), pstate.word.c_str());
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


int Config::getTics() const {return tics;}

void Config::setTics(int t)
{
  tics = t;
  tmt::log(logDEBUG, _("tics set to %d"), tics);
}

int Config::getWidth() const {return width;}

void Config::setWidth(int t)
{
  width = t;
  tmt::log(logDEBUG, _("width set to %d"), width);
}

int Config::getHeight() const {return height;}

void Config::setHeight(int t)
{
  height = t;
  tmt::log(logDEBUG, _("height set to %d"), height);
}

Config::Chips Config::getChips() const {return chips;}

// TODO(fb): this parameter is useless, and redundant, as it can be derived
// from the species. we need to remove it in the future.
void Config::setChips(const Chips &chps)
{
  chips = chps;
  if (FILELog::ReportingLevel() >= logDEBUG)
  {
    FILE_LOG(logDEBUG) << "CHIPS=|" << tmt::dumpMapStringInt(chips);
  }
}

Config::Species Config::getSpecies() const {return species;}

void Config::setSpecies(const Species &spcs)
{
  species = spcs;
  if (FILELog::ReportingLevel() >= logDEBUG) {
    for (auto sp : species) {
      FILE_LOG(logDEBUG) << "SPECIES=" << sp.first << " -> |"
                         << tmt::dumpMapStringInt(sp.second);
    }
  }
}

Config::Positions Config::getTermitePositions() const {return termitePositions;}

void Config::setTermitePositions(const Config::Positions &tpos)
{
  termitePositions = tpos;
}

Config::Positions Config::getChipPositions() const {return chipPositions;}

void Config::setChipPositions(const Config::Positions &cpos)
{
  chipPositions = cpos;
}

bool Config::getInitialized() const {return initialized;}

void Config::setInitialized()
{
  initialized = true;
}

void Config::storeEntityPosition(Config::Positions &store, const TmpString &word,
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
    tmt::log(logERROR, _("Unknown entity store."));
  FILE_LOG(logDEBUG) << entity << ": " << ent.species << " " << ent.row
                     << ", " << ent.col;
}

bool Config::read(std::string const& configFile)
{
  if (initialized)
  {
    tmt::log(logERROR, _("Config already initialized."));
    return false;
  }

  std::ifstream file;
  file.open(configFile);
  if (!file.good())
  {
    tmt::log(logERROR, _("Cannot read configuration file: %s."),
             configFile.c_str());
    return false;
  }
  bool finished = parserRun(file);
  file.close();

  initialized = true;
  bool checked = check();
  return finished && checked;
}

bool Config::read(std::istringstream& config)
{
  if (initialized)
  {
    tmt::log(logERROR, _("Config already initialized."));
    return false;
  }
  bool finished = parserRun(config);
  initialized = true;
  bool checked = check();
  return finished && checked;
}

bool Config::parserRun(std::istream& stream)
{
  ParserState pstate;
  parserInit(pstate);
  while (!stream.eof())
  {
    stream.read(pstate.buffer, BUFFER_SIZE);
    parserExecute(pstate, pstate.buffer, stream.gcount());
  }
  return parserFinish(pstate);
}

bool Config::check() const
{
  return checkParamsDefined() && checkSpeciesChipsCoherence() &&
    checkSpeciesAndBounds() && checkInitialPositions();
}

bool Config::checkParamsDefined() const
{
  struct param_check_t { const char* msg; int val; };
  param_check_t params[] = {
    { _("Undefined parameter 'tics'."), tics },
    { _("Undefined parameter 'height'."), height },
    { _("Undefined parameter 'width'."), width },
    { _("Undefined parameter 'chips'."), int(chips.size()) },
    { _("Undefined parameter 'species'."), int(species.size()) },
    { _("Missing termite positions."), int(termitePositions.size()) },
    { _("Missing chip positions."), int(chipPositions.size()) },
  };
  for (auto p : params) {
    if (!p.val) {
      FILE_LOG(logERROR) << p.msg;
      return false;
    };
  }
  return true;
}

bool Config::checkSpeciesAndBounds() const
{
  struct member_t { const Config::Positions* positions; const Species* spc;
    const Chips* chp; const char* msg; };
  member_t members[] = {
    { &termitePositions, &species, nullptr, "species" },
    { &chipPositions, nullptr, &chips, "chips" },
  };

  for (auto mbr : members) {
    for (auto pos : *(mbr.positions)) {
      if ((mbr.spc && (*mbr.spc).find(pos.species) == (*mbr.spc).end()) ||
          (mbr.chp && (*mbr.chp).find(pos.species) == (*mbr.chp).end()))
      {
        tmt::log(logERROR, _("Wood species '%s' (at %d, %d) is not defined in "
                             "global species."),
                 pos.species.c_str(), pos.row, pos.col);
        return false;
      }

      if ((pos.row > height) || (pos.col > width))
      {
        tmt::log(logERROR, _("Position out of bounds (at %d, %d)."),
          pos.row, pos.col);
        return false;
      }
    }
  }
  return true;
}

bool Config::checkInitialPositions() const
{
  std::vector<bool> cells(width*height);

  for (auto positionSet : { &termitePositions, &chipPositions }) {
    for (auto pos : *positionSet) {
      int rank = tmt::coord2DToRank(pos.row, pos.col, width);
      if (cells[rank]) {
        tmt::log(logERROR,
                 _("Cell (%d, %d) already occupied."), pos.row, pos.col);
        return false;
      }
      else {
        cells[rank] = true;
      }
    }
  }
  return true;
}

bool Config::checkSpeciesChipsCoherence() const
{
  Chips woods;
  for (auto sp : species)
    for (auto wood : sp.second)
      woods[wood.first]++;

  for (auto w : woods)
  {
    auto lookup = chips.find(w.first);
    if (lookup == chips.end()) {
      tmt::log(logERROR, _("Missing chip definition: %s"), w.first.c_str());
      return false;
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
  tmt::log(logDEBUG, _("Config has errors: %s."), tmt::btos(err).c_str());
  tmt::log(logDEBUG, _("Config parsed to the end: %s."), tmt::btos(fin).c_str());
  return !err && fin;
}

bool Config::parserHasError(ParserState &pstate) {
  return pstate.fail || pstate.cs == %%{ write error; }%%;
}

bool Config::parserIsFinished(ParserState &pstate) {
  return !pstate.fail && pstate.cs >= %%{ write first_final; }%%;
}
