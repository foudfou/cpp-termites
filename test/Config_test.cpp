#include "ext/catch.hpp"
#include "Config.hpp"

TEST_CASE( "Config", "[config]" ) {
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
  conf.setChipPositions({ {"pine",7,7}, {"pine",5,5}, {"balsa",6,6}, {"balsa",3,2}, {"oak",6,7} });

  SECTION( "missing parameter" ) {
    REQUIRE( conf.checkParamsDefined() == false );
    REQUIRE( conf.check() == false );
  }

  conf.setTics(40);
  SECTION( "checks success" ) {
    REQUIRE( conf.checkParamsDefined() == true );
    REQUIRE( conf.checkSpeciesChipsCoherence() == true );
    REQUIRE( conf.checkSpeciesAndBounds() == true );
    REQUIRE( conf.check() == true );
  }

  SECTION( "bound checks fail" ) {
    conf.setChipPositions({ {"pine",7,7}, {"pine",5,5}, {"balsa",6,6}, {"balsa",3,2}, {"oak",7,8} });
    REQUIRE( conf.checkSpeciesAndBounds() == false );
    REQUIRE( conf.check() == false );
  }

  SECTION( "missing species definition" ) {
    conf.setTermitePositions({ {"red",5,4}, {"brown",1,1}, {"blue",2,6} });
    REQUIRE( conf.checkSpeciesAndBounds() == false );
    REQUIRE( conf.check() == false );
  }

  SECTION( "check species/chips coherence" ) {
  conf.setChips({ {"pine",0}, {"balsa",1}, {"oak",2} });
    REQUIRE( conf.checkSpeciesAndBounds() == true );
    REQUIRE( conf.checkSpeciesChipsCoherence() == false );
    REQUIRE( conf.checkSpeciesAndBounds() == true );
    REQUIRE( conf.check() == false );
  }

}
