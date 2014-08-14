/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <string>
#include "ext/log.h"

#define _(String) String      // needed for xgettext

namespace tmt {

  typedef size_t idx_t;

  // TODO: if we want an infinite board in the future, we probably want int's,
  // and the board centered around Position({0,0}).
  struct Position {
    unsigned col;
    unsigned row;
    bool operator==(const Position& rhs) const;
    bool operator<(const Position& rhs) const;
  };

  std::ostream& operator<<(std::ostream& os, const Position& pos);

  static const int MSG_MAX_LEN = 512;

  inline std::mt19937& random_gen() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
  }

  void log(TLogLevel level, const char* msg, ...);

  std::string btos(const bool& b);

  std::string dumpMapStringInt(const std::map<std::string, int>& m);

  std::map<std::string, int> listStringToMapStringInt(std::list<std::string> l);

  std::vector<int> pickn(const int n, const int len);

  Position rankToPosition(const idx_t& n, const unsigned& width);

  idx_t positionToRank(const Position& pos, const unsigned& width);

  template<
    template<class T, class All = std::allocator<T> > class ContainerT,
    typename PtrT>
  PtrT find(const ContainerT<PtrT>& ctnr, const std::string& entry)
  {
    auto it =
      std::find_if(
      ctnr.begin(), ctnr.end(),
      [entry](PtrT const& i){ return i.get()->getName() == entry; }
      );
    return (it != ctnr.end()) ? *it : nullptr;
  }

}

#endif /* _HELPERS_H_ */
