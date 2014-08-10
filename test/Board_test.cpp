#include "ext/catch.hpp"
#include <iostream>
#include "Board.hpp"
#include "Termite.hpp"

TEST_CASE( "Board", "[world]" ) {
  Board b;
  b.resize({2,3});
  REQUIRE( !b({0,0}) );
  REQUIRE( !b({1,2}) );
  b({1,1}) = Board::PiecePtr(new Termite);
  std::ostringstream out;
  out << b({1,1})->dump();
  REQUIRE( out.str() == "Termite");
}
