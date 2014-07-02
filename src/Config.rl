/* -*- mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
#include "Config.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.hpp"
#include "log.h"

/* https://github.com/ironbee/ironbee/blob/master/engine/config-parser.rl */
Config::Config() {}

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

%%{

  machine termites_conf;

  action error_any {
    FILE_LOG(logERROR) << "Parse error at line " << lineCount;
    fbreak;
    /* fhold; */
    /* fgoto line; */
  }

  action line_count_inc {
    ++lineCount;
  }

  action mark {
    tok = fpc;
  }

  action time_def {
    tmp_str = strndup(tok, fpc - tok);
    setTime(std::stoi(tmp_str));
    safefree((void **)&tmp_str);
  }

  // FIXME: is there no way to refactor *_def actions ?
  action width_def {
    tmp_str = strndup(tok, fpc - tok);
    setWidth(std::stoi(tmp_str));
    safefree((void **)&tmp_str);
  }

  action height_def {
    tmp_str = strndup(tok, fpc - tok);
    setHeight(std::stoi(tmp_str));
    safefree((void **)&tmp_str);
  }

  action list_init {
    tmp_list.clear();
    tmp_str = strndup(tok, fpc - tok);
    tmp_list[tmp_str]++;
    safefree((void **)&tmp_str);
  }

  action list_append {
    tmp_str = strndup(tok, fpc - tok);
    tmp_list[tmp_str]++;
    safefree((void **)&tmp_str);
  }

  action chips_def {
    setChips(tmp_list);
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

void Config::setChips(const std::map<std::string, int>& chps)
{
  chips = chps;
  if (FILELog::ReportingLevel() >= logDEBUG) {
    std::string chipsStr;
    for (auto chip: chips) chipsStr += chip.first + '|';
    FILE_LOG(logDEBUG) << "CHIPS=|" << chipsStr;
  }
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

  const char *tok = nullptr;
  char *tmp_str = nullptr;
  std::map<std::string, int> tmp_list;

  %% write init;

  %% write exec;

  assert(tmp_str == nullptr && "tmp must be cleared after every use");

  FILE_LOG(logINFO) << "Config has errors: " << btos(parserHasError(cs));

  return parserIsFinished(cs);
}

bool Config::parserHasError(int cs) {
  return cs == %%{ write error; }%%;
}

bool Config::parserIsFinished(int cs) {
  return cs >= %%{ write first_final; }%%;
}
