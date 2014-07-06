/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <sys/stat.h>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>
#include "log.h"

#include "helpers.hpp"

namespace tmt {

  long fileSize(const std::string& filename)
  {
    struct stat statBuf;
    int ret = stat(filename.c_str(), &statBuf);
    return ret == 0 ? statBuf.st_size : -1;
  }

  std::string btos(const bool& b)
  {
    return b ? "true" : "false";
  }

  std::string mapToString(const std::map<std::string, int>& m)
  {
    std::string str;
    for (auto e : m) str += e.first + '|';
    return str;
  }

  std::map<std::string, int> listToMap(std::list<std::string> l)
  {
    std::map<std::string, int> m;
    std::list<std::string>::const_iterator it(l.begin()), end(l.end());
    for (; it != end; ++it)
      if (m[*it])
        FILE_LOG(logWARNING) << "Duplicate item: " << *it;
      else
        m[*it]++;
    return m;
  }

  /* http://stackoverflow.com/a/16421677/421846 */
  template<typename Iter, typename RandomGenerator>
  Iter pick(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
  }

  template<typename Iter>
  Iter pick(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
  }

  std::vector<int> pickn(const int n, const int len)
  {
    std::vector<int> vec(len);
    std::iota(std::begin(vec), std::end(vec), 0);
    std::shuffle(vec.begin(), vec.end(), tmt::gen);

    std::vector<int>::const_iterator first = vec.begin();
    std::vector<int>::const_iterator last = vec.begin() + n;
    std::vector<int> slice(first, last);
    return slice;
  }

  std::vector< std::pair<int, int> >
  randomIntPairs(const int n, const int h, const int w)
  {
    std::uniform_int_distribution<> hdis(0, h);
    std::uniform_int_distribution<> wdis(0, w);
    std::vector< std::pair<int, int> > randoms;
    for (int i=0; i<n; ++i)
      randoms.push_back(std::pair<int, int>(hdis(gen), wdis(gen)));
    return randoms;
  }

  std::pair<int, int> position(const int n, const int width)
  {
    int row = n / width, col = n % width;
    return std::pair<int, int>(row, col);
  }
}
