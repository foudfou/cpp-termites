/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <list>
#include <map>
#include <random>
#include <string>

namespace tmt {

  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::string btos(const bool& b);

  std::string dumpMapStringInt(const std::map<std::string, int>& m);

  std::map<std::string, int> listStringToMapStringInt(std::list<std::string> l);

  std::vector<int> pickn(const int n, const int len);

  std::pair<int, int> rankTo2DCoord(const int n, const int width);

}

#endif /* _HELPERS_H_ */
