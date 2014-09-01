/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <libintl.h>
#include <stdarg.h>
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

  bool Position::operator<(const Position& rhs) const
  {
    /* Don't do (col < rhs.col || row < rhs.row), because {5,2}<{5,3} should
       return true */
    if (col != rhs.col) return col < rhs.col;
    return row < rhs.row;
  }

  std::ostream& operator<<(std::ostream& os, const Position& pos)
  {
    return os << "{" << pos.col << "," << pos.row << "}";
  }

  /** Localized logging. */
  void log(TLogLevel level, const char* msg, ...)
  {
    char buff[MSG_MAX_LEN];
    va_list args;
    va_start (args, msg);
    vsnprintf(buff, MSG_MAX_LEN, gettext(msg), args);
    FILE_LOG(level) << buff;
    va_end (args);
  }

  /** Converts a boolean to a "true" or "false" string. */
  std::string btos(const bool& b)
  {
    return b ? "true" : "false";
  }

  /** Returns a strings comprising keys of a map(string->int). */
  std::string dumpMapStringInt(const std::map<std::string, int>& m)
  {
    std::string str;
    for (auto e : m) str += e.first + '|';
    return str;
  }

  /** Converts a list of strings to a map(string,int). */
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

  /** Pick randomly `n` integers among a a list from 0 to `len`, without
   * putting back. */
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

  /** Converts a (integer) rank/index inside a vector representing a grid of
   * witdth `width` to a `Position` coordinate.  */
  Position rankToPosition(const idx_t& n, const unsigned& width)
  {
    if (width < 1) throw std::invalid_argument("width < 1");
    unsigned row = n / width, col = n % width;
    return {col, row};
  }

  /** Converts a `Position` coordinate to a (integer) rank/index inside a
   * vector representing a grid of witdth `width`. */
  idx_t positionToRank(const Position& pos, const unsigned& width)
  {
    if (width < 1) throw std::invalid_argument("width < 1");
    if (pos.col >= width) throw std::invalid_argument("row out of bound");
    return pos.row * width + pos.col;
  }

  /** Split a range [`lo`, `hi`] into `n` slices */
  std::vector<int> splitRange(const int lo, const int hi, const int n)
  {
    if (lo >= hi) throw std::invalid_argument("lo >= hi");
    if (n < 1) throw std::invalid_argument("n < 1");
    if (n == 1) return std::vector<int>({hi});

    std::vector<int> vec(n);
    int delta = (hi - lo)/(n - 1);
    vec[0] = lo;
    for (int i=1; i<n; ++i)
      vec[i] = lo + delta*i;
    return vec;
  }

}
