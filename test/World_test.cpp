#include "ext/catch.hpp"
#include "Config.hpp"
#include "World.hpp"
#include "test_helpers.hpp"
#include "fixtures/ConfigFixt.hpp"

TEST_CASE( "World is a singleton", "[world]" ) {
  World& w1 = World::getInstance();
  World& w2 = World::getInstance();
  REQUIRE( &w1 == &w2 );
}

TEST_CASE( "World setup", "[world]" ) {
  tmt::LogCapture logCap;
  std::shared_ptr<Config> conf = std::make_shared<Config>(confFixt1());
  World& w1 = World::getInstance();
  w1.populate(conf);
  REQUIRE( w1.getWoodSpecies("pine") );
  REQUIRE( w1.getWoodSpecies("cypress") );
  REQUIRE( !w1.getWoodSpecies("NOWSPECIES") );
  REQUIRE( w1.getWoodSpeciesSize() == 5 );
  REQUIRE( w1.getTermiteSpecies("brown") );
  REQUIRE( w1.getTermiteSpecies("red") );
  REQUIRE( !w1.getWoodSpecies("NOTSPECIES") );
  REQUIRE( w1.getTermiteSpeciesSize() == 3 );
  auto brd = R"(-------
-T-----
--WW---
-------
-----T-
-----W-
------W
------W)";
  REQUIRE( w1.board.dump() == brd );
}
