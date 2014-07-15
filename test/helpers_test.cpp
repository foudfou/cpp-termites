#include "ext/catch.hpp"
#include "helpers.hpp"

TEST_CASE( "Bool to string", "[helpers]" ) {
  REQUIRE( tmt::btos(true) == "true" );
  REQUIRE( tmt::btos(false) == "false" );
}

TEST_CASE( "Map(string->int) dump", "[helpers]" ) {
  const std::map<std::string, int> m1 { {"can", 200}, {"be", 40}, };
  REQUIRE( (tmt::dumpMapStringInt(m1) == "be|can|" ||
            tmt::dumpMapStringInt(m1) == "can|be|") );
}

TEST_CASE( "List(string) to map(string->int)", "[helpers]" ) {
  std::list<std::string> l1 { "can", "be" };
  const std::map<std::string, int> m1 { {"can", 1}, {"be", 1}, };
  REQUIRE( tmt::listStringToMapStringInt(l1) == m1 );
  SECTION( "duplicate keys are ignored" ) {
    l1.push_back("be");
    REQUIRE( tmt::listStringToMapStringInt(l1) == m1 );
  }
}

TEST_CASE( "Pick n integers among N.", "[helpers]" ) {
  std::vector<int> v1 { 0 };
  REQUIRE( tmt::pickn(0, 0) == std::vector<int>() );
  REQUIRE_THROWS( tmt::pickn(1, 0) );
  v1 = tmt::pickn(2, 3);
  REQUIRE( v1.size() == 2 );
  REQUIRE( v1.at(0) != v1.at(1) );
  REQUIRE( (v1.at(0) + v1.at(1)) < (2 * 3) );
}

TEST_CASE( "Rank to Coord2D.", "[helpers]" ) {
  REQUIRE_THROWS( tmt::rankToCoord2D(0, 0) );
  REQUIRE( tmt::rankToCoord2D(0, 1) == (std::pair<int, int>(0, 0)) );
  REQUIRE( tmt::rankToCoord2D(0, 3) == (std::pair<int, int>(0, 0)) );
  REQUIRE( tmt::rankToCoord2D(4, 3) == (std::pair<int, int>(1, 1)) );
  REQUIRE( tmt::rankToCoord2D(3, 3) == (std::pair<int, int>(1, 0)) );
}

TEST_CASE( "Coord2D to Rank.", "[helpers]" ) {
  REQUIRE_THROWS( tmt::coord2DToRank(0, 0, 0) ); // zero-width
  REQUIRE_THROWS( tmt::coord2DToRank(6, 1, 5) );
  REQUIRE( tmt::coord2DToRank(0, 1, 5) == 5 );
  REQUIRE( tmt::coord2DToRank(2, 0, 5) == 2 );
  REQUIRE( tmt::coord2DToRank(2, 2, 5) == 12 );
}
