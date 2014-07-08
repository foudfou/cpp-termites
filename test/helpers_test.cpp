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
  const std::list<std::string> l1 { "can", "be" };
  const std::map<std::string, int> m1 { {"can", 1}, {"be", 1}, };
  REQUIRE( tmt::listStringToMapStringInt(l1) == m1 );
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

TEST_CASE( "Rank to 2DCoord.", "[helpers]" ) {
  REQUIRE_THROWS( tmt::rankTo2DCoord(0, 0) );
  REQUIRE( tmt::rankTo2DCoord(0, 1) == (std::pair<int, int>(0, 0)) );
  REQUIRE( tmt::rankTo2DCoord(0, 3) == (std::pair<int, int>(0, 0)) );
  REQUIRE( tmt::rankTo2DCoord(4, 3) == (std::pair<int, int>(1, 1)) );
  REQUIRE( tmt::rankTo2DCoord(3, 3) == (std::pair<int, int>(1, 0)) );
}
