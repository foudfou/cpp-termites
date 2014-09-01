/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "ext/catch.hpp"
#include "WoodSpecies.hpp"
#include "TermiteSpecies.hpp"
#include "Piece.hpp"
#include "Termite.hpp"
#include "WoodChip.hpp"

TEST_CASE( "Termite", "[game]" ) {
  WoodSpeciesPtr wspc1(new WoodSpecies("oak"));
  TermiteSpeciesPtr tspc1(new TermiteSpecies("brown", {wspc1}));
  PiecePtr w1 = PiecePtr(new WoodChip(wspc1));
  Termite* t1 = new Termite(tspc1);
  t1->load(w1);
  REQUIRE_THROWS( t1->load(w1) );
  PiecePtr w2 = t1->unload();
  REQUIRE( w2 == w1 );
  delete t1;
}
