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
