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

  bool Position::operator==(const Position& rhs) const
  {
    return col == rhs.col && row == rhs.row;
  }

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
    std::shuffle(vec.begin(), vec.end(), random_gen());

    std::vector<int>::const_iterator first = vec.begin();
    std::vector<int>::const_iterator last = vec.begin() + n;
    std::vector<int> slice(first, last);
    return slice;
  }

  Position rankToPosition(const unsigned n, const unsigned width)
  {
    if (width < 1) throw std::invalid_argument("width < 1");
    unsigned row = n / width, col = n % width;
    return {col, row};
  }

  unsigned positionToRank(const Position pos, const unsigned width)
  {
    if (width < 1) throw std::invalid_argument("width < 1");
    if (pos.col >= width) throw std::invalid_argument("row out of bound");
    return pos.row * width + pos.col;
  }

}
