/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "test_helpers.hpp"

namespace tmt {

  CoutCapture::CoutCapture(std::streambuf* buff) :
    old(std::cout.rdbuf(buff)) {}

  CoutCapture::~CoutCapture() {std::cout.rdbuf(old);}

  LogCapture::LogCapture(): buffer(nullptr), bufferSize(0)
  {
    bak = Output2FILE::Stream();
    ss = open_memstream(&buffer, &bufferSize);
    Output2FILE::Stream() = ss;
  }

  LogCapture::~LogCapture()
  {
    Output2FILE::Stream() = bak;
    fflush(ss);
    fclose(ss);
    free(buffer);
  }

  std::string LogCapture::getBuffer() const {return std::string(buffer);}

  int alen(const char**ary)
  {
    int count = 0;
    const int MAX = 128;
    while (count < MAX)
    {
      if (!ary[count]) return count;
      count++;
    }
    return -1;
  }

}
