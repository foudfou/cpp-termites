/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <sys/stat.h>
#include <random>
#include "log.h"

#include "helpers.hpp"


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

void randInt(const int a, const int b)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(a, b);

  // for (int n=0; n<10; ++n)
  //   std::cout << dis(gen) << ' ';
  // std::cout << '\n';
}
