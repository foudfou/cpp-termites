#include "ext/catch.hpp"
#include "TermiteSpecies.hpp"

TEST_CASE( "TermiteSpecies find", "[world]" ) {
  WoodSpeciesPtr wspc1(new WoodSpecies("oak"));
  WoodSpeciesPtr wspc2(new WoodSpecies("maple"));
  WoodSpeciesPtr wspc3(new WoodSpecies("birch"));
  TermiteSpeciesPtr tspc1(new TermiteSpecies("brown", {wspc1,wspc2,wspc3}));
  REQUIRE( tspc1->likes("maple") );
}
