#include <string>
#include "ext/catch.hpp"
#include "Config.hpp"
#include "config.h"

TEST_CASE( "Bare Config", "[config]" ) {
  Config conf;
  conf.setWidth(7);
  conf.setHeight(8);
  conf.setChips({ {"pine",0}, {"balsa",1}, {"oak",2}, {"ebony",1},
    {"cypress",1} });
  conf.setSpecies({
    {"red",   { {"pine",0}, {"balsa",1}, {"oak",2} } },
    {"brown", { {"balsa",0}, {"ebony",1} } },
    {"green", { {"cypress",0} } },
  });
  conf.setTermitePositions({ {"red",5,4}, {"brown",1,1}, });
  conf.setChipPositions({ {"pine",7,7}, {"pine",5,5}, {"balsa",6,6},
      {"balsa",3,2}, {"oak",6,7} });

  SECTION( "missing parameter" ) {
    REQUIRE( !conf.checkParamsDefined() );
    REQUIRE( !conf.check() );
  }

  conf.setTics(40);
  SECTION( "checks success" ) {
    REQUIRE( conf.checkParamsDefined() );
    REQUIRE( conf.checkSpeciesChipsCoherence() );
    REQUIRE( conf.checkSpeciesAndBounds() );
    REQUIRE( conf.check() );
  }

  SECTION( "bound checks fail" ) {
    conf.setChipPositions({ {"pine",7,7}, {"pine",5,5}, {"balsa",6,6},
        {"balsa",3,2}, {"oak",7,8} });
    REQUIRE( !conf.checkSpeciesAndBounds() );
    REQUIRE( !conf.check() );
  }

  SECTION( "missing species definition" ) {
    conf.setTermitePositions({ {"red",5,4}, {"brown",1,1}, {"blue",2,6} });
    REQUIRE( !conf.checkSpeciesAndBounds() );
    REQUIRE( !conf.check() );
  }

  SECTION( "check species/chips coherence" ) {
    conf.setChips({ {"pine",0}, {"balsa",1}, {"oak",2} });
    REQUIRE( conf.checkSpeciesAndBounds() );
    REQUIRE( !conf.checkSpeciesChipsCoherence() );
    REQUIRE( conf.checkSpeciesAndBounds() );
    REQUIRE( !conf.check() );
  }

  SECTION( "check initial positions" ) {
    REQUIRE( conf.checkInitialPositions() );
    conf.setTermitePositions({ {"red",5,4}, {"brown",1,1}, {"brown",7,7},});
    REQUIRE( !conf.checkInitialPositions() );
  }
}

TEST_CASE( "Config from file", "[config]" ) {
  Config conf;
  REQUIRE( conf.read(std::string(TEST_DIR) + "/fixtures/init_ok.cfg") );
  CHECK( conf.getTics() == 2000 );
  CHECK( conf.getWidth() == 80 );
  CHECK( conf.getHeight() == 50 );
  CHECK( conf.getChips() == Config::Chips({
        {"boulot",1}, {"chene",1}, {"baobab",1}, {"noyer",1}, }) );
  CHECK( conf.getSpecies() == Config::Species({
        {"rouge", { {"boulot",1}, } },
        {"verte", { {"boulot",1}, {"chene",1} } },
        {"noire", { {"baobab",1}, {"boulot",1} } },
          }) );
  CHECK( conf.getTermitePositions() == Config::Positions({ {"rouge",10,5},
        {"rouge",23,34} }) );
  CHECK( conf.getChipPositions() == Config::Positions({ {"boulot",42,23},
        {"baobab",1,3} }) );
}
