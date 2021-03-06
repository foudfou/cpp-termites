/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "ext/catch.hpp"
#include "TermiteSpecies.hpp"

TEST_CASE( "TermiteSpecies find", "[game]" ) {
  WoodSpeciesPtr wspc1(new WoodSpecies("oak"));
  WoodSpeciesPtr wspc2(new WoodSpecies("maple"));
  WoodSpeciesPtr wspc3(new WoodSpecies("birch"));
  TermiteSpeciesPtr tspc1(new TermiteSpecies("brown", {wspc1,wspc2}));
  REQUIRE( tspc1->likes(wspc2) );
  REQUIRE( !tspc1->likes(wspc3) );
}
