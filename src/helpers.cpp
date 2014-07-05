#include <sys/stat.h>
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
