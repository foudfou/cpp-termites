/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <libintl.h>
#include <stdarg.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <exception>
#include <iterator>
#include <numeric>
#include <vector>
#include "helpers.hpp"

namespace tmt {

  void log(TLogLevel level, const char* msg, ...)
  {
    char buff[MSG_MAX_LEN];
    va_list args;
    va_start (args, msg);
    vsnprintf(buff, MSG_MAX_LEN, gettext(msg), args);
    FILE_LOG(level) << buff;
    va_end (args);
  }

  std::string btos(const bool& b)
  {
    return b ? "true" : "false";
  }

  std::string dumpMapStringInt(const std::map<std::string, int>& m)
  {
    std::string str;
    for (auto e : m) str += e.first + '|';
    return str;
  }

  std::map<std::string, int> listStringToMapStringInt(std::list<std::string> l)
  {
    std::map<std::string, int> m;
    std::list<std::string>::const_iterator it(l.begin()), end(l.end());
    for (; it != end; ++it)
      if (m[*it])
        log(logWARNING, _("Duplicate item: %s."), (*it).c_str());
      else
        m[*it]++;
    return m;
  }

  std::vector<int> pickn(const int n, const int len)
  {
    if (n > len) throw std::invalid_argument("n > length");
    std::vector<int> vec(len);
    std::iota(std::begin(vec), std::end(vec), 0);
    std::shuffle(vec.begin(), vec.end(), tmt::gen);

    std::vector<int>::const_iterator first = vec.begin();
    std::vector<int>::const_iterator last = vec.begin() + n;
    std::vector<int> slice(first, last);
    return slice;
  }

  std::pair<int, int> rankToCoord2D(const int n, const int width)
  {
    if (width < 1) throw std::invalid_argument("width < 1");
    int row = n / width, col = n % width;
    return std::pair<int, int>(row, col);
  }

  int coord2DToRank(const int row, const int col, const int width)
  {
    if (width < 1) throw std::invalid_argument("width < 1");
    if (row > width) throw std::invalid_argument("row out of bound");
    return col * width + row;
  }

}
