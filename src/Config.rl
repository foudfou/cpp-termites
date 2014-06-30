/* -*- mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
#include "Config.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
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
    FILE_LOG(logERROR) << "Parse error at line " << line_count;
    fbreak;
    /* fhold; */
    /* fgoto line; */
  }

  action line_count_inc {
    ++line_count;
    FILE_LOG(logDEBUG) << "line=" << line_count;
  }

  action start_value {
    tok = fpc;
    FILE_LOG(logDEBUG) << "START at=" << *tok;
  }
  action write_value {
    char *num;
    int len;
    FILE_LOG(logDEBUG) << "tok1=" << *tok;
    len = fpc - tok;
    FILE_LOG(logDEBUG) << "size=" << len;
    num = strndup(tok, len);
    FILE_LOG(logDEBUG) << "tok2=" << num;
    free(num);
    FILE_LOG(logDEBUG) << "LEAVING";
  }

  include termites_conf_core "rl/termites_conf.rl";

}%%

%% write data;


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
  p = conf.c_str();         // pointer begin
  pe = p + strlen(p) + 1;   // pointer end
  char *eof = NULL;
  int line_count = 0;
  // FILE_LOG(logDEBUG) << "conf=" << p;

  const char *tok;

  %% write init;

  %% write exec;

  FILE_LOG(logINFO) << "ERR=" << (cs == termites_conf_error);

  return true;
}
