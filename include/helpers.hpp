/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <list>
#include <map>
#include <random>
#include <string>
#include "ext/log.h"

#define _(String) String      // needed for xgettext

namespace tmt {

  static const int MSG_MAX_LEN = 512;

  static std::random_device rd;
  static std::mt19937 gen(rd());

  void log(TLogLevel level, const char* msg, ...);

  std::string btos(const bool& b);

  std::string dumpMapStringInt(const std::map<std::string, int>& m);

  std::map<std::string, int> listStringToMapStringInt(std::list<std::string> l);

  std::vector<int> pickn(const int n, const int len);

  std::pair<int, int> rankToCoord2D(const int n, const int width);

  int coord2DToRank(const int row, const int col, const int width);

}

#endif /* _HELPERS_H_ */
