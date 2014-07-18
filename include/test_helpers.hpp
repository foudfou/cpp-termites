#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <cstdio>
#include <string>
#include "ext/log.h"

namespace tmt {

  class LogCapture {
  public:
    LogCapture();
    ~LogCapture();

    std::string getBuffer();

  private:
    char* buffer;
    size_t bufferSize;
    FILE* bak;
    FILE* ss;
  };

  int alen(const char**ary);

}

#endif /* TEST_HELPERS_H */
