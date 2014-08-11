#include "ext/catch.hpp"
#include <iostream>
#include "Board.hpp"
#include "Termite.hpp"

TEST_CASE( "Board", "[world]" ) {
  Board b;
  b.resize({2,3});
  REQUIRE( !b({0,0}) );
  REQUIRE( !b({1,2}) );
  WoodSpeciesPtr wspc1(new WoodSpecies("oak"));
  TermiteSpeciesPtr tspc1(new TermiteSpecies("brown", {wspc1}));
  b({1,1}) = Board::PiecePtr(new Termite(tspc1));
  std::ostringstream out;
  out << b({1,1})->dump();
  REQUIRE( out.str() == "Termite");
}
