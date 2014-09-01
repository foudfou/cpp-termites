/* Copyright (c) 2014 Foudil Brétel. All rights reserved. */

#include "ext/catch.hpp"
#include <iostream>
#include "test_helpers.hpp"
#include "Board.hpp"
#include "Piece.hpp"
#include "Termite.hpp"
#include "WoodChip.hpp"

TEST_CASE( "Board", "[game]" ) {
  Board b;
  b.resize({2,3});
  REQUIRE( !b({0,0}) );
  REQUIRE( !b({1,2}) );
  WoodSpeciesPtr wspc1(new WoodSpecies("oak"));
  TermiteSpeciesPtr tspc1(new TermiteSpecies("brown", {wspc1}));
  b({1,1}) = PiecePtr(new Termite(tspc1));
  std::ostringstream out;
  out << b({1,1})->dump();
  REQUIRE( out.str() == "Termite");
  auto termites = b.getTermitePositions();
  REQUIRE( termites.size() == 1 );
  REQUIRE( b(termites[0])->isTermite() );
}

TEST_CASE( "Surrounding positions", "[game]" ) {
  Board b;
  b.resize({4,5});

  SECTION( "inside cell" ) {
    std::vector<tmt::Position> positions = b.getAdjacentPositions({1,1});
    REQUIRE( positions.size() == 8 );
    REQUIRE( positions[0] == tmt::Position({0,0}) );
    REQUIRE( positions[7] == tmt::Position({2,2}) );
  }

  SECTION( "corner cell" ) {
    std::vector<tmt::Position> positions = b.getAdjacentPositions({3,0});
    REQUIRE( positions.size() == 3 );
    REQUIRE( positions[0] == tmt::Position({2,0}) );
    REQUIRE( positions[1] == tmt::Position({2,1}) );
    REQUIRE( positions[2] == tmt::Position({3,1}) );
  }
}

TEST_CASE( "Intersect adjacent positions", "[game]" ) {
  Board b;
  b.resize({10,7});
  auto inter = b.intersectAdjacentPositions(tmt::Position({1,1}),
                                            tmt::Position({0,0}));
  REQUIRE( inter == std::vector<tmt::Position>({{0,1},{1,0}}) );
  inter = b.intersectAdjacentPositions(tmt::Position({0,0}),
                                       tmt::Position({1,1}));
  REQUIRE( inter == std::vector<tmt::Position>({{0,1},{1,0}}) );
  inter = b.intersectAdjacentPositions(tmt::Position({5,3}),
                                       tmt::Position({4,4}));
  REQUIRE( inter == std::vector<tmt::Position>({{4,3},{5,4}}) );
  inter = b.intersectAdjacentPositions(tmt::Position({4,4}),
                                       tmt::Position({5,3}));
  REQUIRE( inter == std::vector<tmt::Position>({{4,3},{5,4}}) );
}
