/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#define CATCH_CONFIG_MAIN  // provide a main() - only do this in one cpp file
#include "ext/catch.hpp"
#include "ext/log.h"

TEST_CASE( "Turn off logging", "[init]" ) {
  FILELog::ReportingLevel() = FILELog::FromString("NONE");
}

// all subsequent tests in <code_file>_test.cpp
