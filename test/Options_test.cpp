#include "ext/catch.hpp"
#include <memory>
#include "test_helpers.hpp"
#include "Config.hpp"
#include "Options.hpp"


TEST_CASE( "Options initialization and parsing", "[options]" ) {
  std::shared_ptr<Config> conf(new Config);
  Options options;
  REQUIRE( options.setConfig(conf) );

  SECTION( "missing options and parameter" ) {
    std::unique_ptr<tmt::LogCapture> logCap(new tmt::LogCapture());
    REQUIRE( !options.parse(1, {}) );
    // FIXME: messages should be constants
    std::string msg("Missing options or configuration file");
    REQUIRE( logCap->getBuffer().find(msg) != std::string::npos );
  }

  SECTION( "options and parameter fails" ) {
    std::unique_ptr<tmt::LogCapture> logCap(new tmt::LogCapture());
    const char * arggv[] =
      {"/path/me","-W","0","-H","7","-t","10","-c","8","-T","20","someFile",NULL};
    size_t len = tmt::alen(arggv);
    REQUIRE( !options.parse(len, (char *const *)arggv) );
    std::string msg("Please provide either a configuration file OR all mandatory options");
    REQUIRE( logCap->getBuffer().find(msg) != std::string::npos );
  }
}
