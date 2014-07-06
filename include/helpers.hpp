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

  long fileSize(const std::string& filename);

  std::string btos(const bool& b);

  std::string mapToString(const std::map<std::string, int>& m);

  std::map<std::string, int> listToMap(std::list<std::string> l);

  template<typename Iter, typename RandomGenerator>
  Iter pick(Iter start, Iter end, RandomGenerator& g);

  template<typename Iter>
  Iter pick(Iter start, Iter end);

  std::vector<int> pickn(const int n, const int len);

  std::vector< std::pair<int, int> >
  randomIntPairs(const int n, const int h, const int w);

  std::pair<int, int> position(const int n, const int width);

}

#endif /* _HELPERS_H_ */
