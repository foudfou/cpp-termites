/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <cstdio>
#include <iostream>
#include <string>
#include "ext/log.h"

namespace tmt {

  // http://stackoverflow.com/a/5419388/421846
  class CoutCapture {
  public:
    CoutCapture(std::streambuf* buff);
    ~CoutCapture();

  private:
    std::streambuf* old;
  };

  class LogCapture {
  public:
    LogCapture();
    ~LogCapture();

    std::string getBuffer() const;

  private:
    char* buffer;
    size_t bufferSize;
    FILE* bak;
    FILE* ss;
  };

  int alen(const char**ary);

}

#endif /* TEST_HELPERS_H */
