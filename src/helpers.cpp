#include <iostream>
#include <map>
#include <sys/stat.h>

#include "helpers.hpp"


long fileSize(const std::string& filename)
{
  struct stat statBuf;
  int ret = stat(filename.c_str(), &statBuf);
  return ret == 0 ? statBuf.st_size : -1;
}

void safefree(void **pp)
{
  free(*pp);
  *pp = nullptr;
}

std::string btos(const bool& b)
{
  return b ? "true" : "false";
}
