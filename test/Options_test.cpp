/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include <memory>
#include "ext/catch.hpp"
#include "helpers.hpp"
#include "test_helpers.hpp"
#include "Config.hpp"
#include "Options.hpp"

TEST_CASE( "Options initialization and parsing", "[options]" ) {
  std::shared_ptr<Config> conf(new Config);
  Options options;
  options.setConfig(conf);
  tmt::LogCapture logCap;

  SECTION( "missing options and parameter" ) {
    REQUIRE( options.parse(1, {}) == tmt::Options::ERROR );
    std::string msg("Missing options or configuration file."); // tests not localized
    REQUIRE( logCap.getBuffer().find(msg) != std::string::npos );
  }

  SECTION( "options and parameter fails" ) {
    const char * argv_[] =
      {"/path/me","-W","0","-H","7","-t","10","-c","8","-T","20","someFile",NULL};
    size_t len = tmt::alen(argv_);
    REQUIRE( options.parse(len, const_cast<char *const *>(argv_)) ==
             tmt::Options::ERROR );
    std::string msg("Please provide either a configuration file or all "
                    "mandatory options (height, width, termites, chips, tics).");
    REQUIRE( logCap.getBuffer().find(msg) != std::string::npos );
  }

  SECTION( "board too small" ) {
    const char * argv_[] =
      {"/path/me","-W","0","-H","0","-t","10","-c","8","-T","20",NULL};
    size_t len = tmt::alen(argv_);
    REQUIRE( options.parse(len, const_cast<char *const *>(argv_)) ==
             tmt::Options::ERROR );
    std::string msg("Too many termites and chips for board size.");
    REQUIRE( logCap.getBuffer().find(msg) != std::string::npos );
  }

  SECTION( "missing options" ) {
    const char * argv_[] = {"/path/me","-W","0",NULL};
    size_t len = tmt::alen(argv_);
    REQUIRE( options.parse(len, const_cast<char *const *>(argv_)) ==
             tmt::Options::ERROR );
    std::string msg("Please provide all options (height, width, termites, chips, tics).");
    REQUIRE( logCap.getBuffer().find(msg) != std::string::npos );
  }

  SECTION( "multiple configuration files" ) {
    const char * argv_[] = {"/path/me","one","two",NULL};
    size_t len = tmt::alen(argv_);
    REQUIRE( options.parse(len, const_cast<char *const *>(argv_)) ==
             tmt::Options::ERROR );
    std::string msg("More than one configuration file given.");
    REQUIRE( logCap.getBuffer().find(msg) != std::string::npos );
  }

  SECTION( "options success" ) {
    const char * argv_[] =
      {"/path/me","-W","10","-H","7","-t","10","-c","8","-T","20",NULL};
    size_t len = tmt::alen(argv_);
    REQUIRE( options.parse(len, const_cast<char *const *>(argv_)) ==
             tmt::Options::SUCCESS );
  }

  SECTION( "version success" ) {
    std::ostringstream out;
    tmt::CoutCapture outCap(out.rdbuf());
    const char* argv_[] = {"/path/me","-v",NULL};
    size_t len = tmt::alen(argv_);
    REQUIRE( options.parse(len, const_cast<char *const *>(argv_)) ==
             tmt::Options::NOOP );
    REQUIRE( out.str().find("Copyright (c)") != std::string::npos );
  }

  SECTION( "debug output to file success" ) {
    const char* argv_[] =
      {"/path/me","-d","-o","/tmp/termites_test.log","/where/ever/init.cfg",NULL};
    size_t len = tmt::alen(argv_);
    REQUIRE( options.parse(len, const_cast<char *const *>(argv_)) ==
             tmt::Options::SUCCESS );
  }

  SECTION( "output to wrong file" ) {
    const char* argv_[] =
      {"/path/me","-d","-o","/no/where/termites_test.log","/where/ever/init.cfg",NULL};
    size_t len = tmt::alen(argv_);
    REQUIRE( options.parse(len, const_cast<char *const *>(argv_)) ==
             tmt::Options::ERROR );
    REQUIRE( logCap.getBuffer().find("Cannot open log file") != std::string::npos );
  }

}
