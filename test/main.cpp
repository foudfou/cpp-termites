/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#define CATCH_CONFIG_RUNNER  // only in one cpp file
#include "ext/catch.hpp"

#include <memory>
#include "test_helpers.hpp"


int main(int argc, char* const argv[])
{
  std::auto_ptr<LogCapture> logCap(new LogCapture());

  int result = Catch::Session().run(argc, argv);

  return result;
}

// all subsequent tests in <code_file>_test.cpp
