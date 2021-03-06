/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "ext/catch.hpp"
#include <memory>
#include <string>
#include "test_helpers.hpp"
#include "Config.hpp"
#include "config.h"
#include "fixtures/ConfigFixt.hpp"

TEST_CASE( "Bare Config", "[config]" ) {
  tmt::LogCapture logCap;
  Config conf = confFixt1();

  conf.setTics(0);
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
    conf.setChipPositions({ {"pine",{2,2}}, {"pine",{5,5}}, {"balsa",{6,6}},
      {"balsa",{3,2}}, {"oak",{7,7}} });
    REQUIRE( !conf.checkSpeciesAndBounds() );
    REQUIRE( !conf.check() );
  }

  SECTION( "missing species definition" ) {
    conf.setTermitePositions({ {"red",{5,4}}, {"brown",{1,1}}, {"blue",{2,6}} });
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
    conf.setTermitePositions({ {"red",{5,4}}, {"brown",{1,1}}, {"brown",{6,7}},});
    REQUIRE( !conf.checkInitialPositions() );
  }
}

TEST_CASE( "Config file ok", "[config]" ) {
  tmt::LogCapture logCap;
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
  CHECK( conf.getTermitePositions() == Config::Positions({ {"rouge",{10,5}},
        {"rouge",{23,34}} }) );
  CHECK( conf.getChipPositions() == Config::Positions({ {"boulot",{42,23}},
        {"baobab",{1,3}} }) );

  SECTION( "Config file read twice fails" ) {
    REQUIRE( !conf.read(std::string(TEST_DIR) + "/fixtures/init_ok.cfg") );
  }
}

TEST_CASE( "Config file void", "[config]" ) {
  tmt::LogCapture logCap;
  Config conf;
  REQUIRE( !conf.read(std::string(TEST_DIR) + "/fixtures/init_void.cfg") );
}

TEST_CASE( "Config string ok", "[config]" ) {
  tmt::LogCapture logCap;
  Config conf;
  std::istringstream ss("temps: 2000\nlargeur: 80\nhauteur: 50\n\n"
    "# should be ignored\n\ncopeaux: ( boulot   chene baobab  boulot  noyer)\n"
    "termites: rouge ( boulot ) verte ( boulot chene ) noire ( baobab boulot )"
    " rouge ( noyer  )\ntermite:   rouge 10 5\ntermite: rouge 23 34\n"
         "copeau: boulot 42 23\ncopeau: baobab 1 3\n");
  REQUIRE( conf.read(ss) );

  SECTION( "Config string read twice fails" ) {
    REQUIRE( !conf.read(ss) );
  }
}

// TODO: add more tests for config syntax
