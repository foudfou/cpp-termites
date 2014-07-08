/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>
#include "log.h"

#include "helpers.hpp"

namespace tmt {

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

  std::pair<int, int> position(const int n, const int width)
  {
    int row = n / width, col = n % width;
    return std::pair<int, int>(row, col);
  }
}
