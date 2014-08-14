#include "ext/catch.hpp"
#include "test_helpers.hpp"
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
  tmt::LogCapture logCap;
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

TEST_CASE( "Rank to Position.", "[helpers]" ) {
  REQUIRE_THROWS( tmt::rankToPosition(0, 0) );
  REQUIRE( tmt::rankToPosition(0, 1) == tmt::Position({0, 0}) );
  REQUIRE( tmt::rankToPosition(0, 3) == tmt::Position({0, 0}) );
  REQUIRE( tmt::rankToPosition(4, 3) == tmt::Position({1, 1}) );
  REQUIRE( tmt::rankToPosition(3, 3) == tmt::Position({0, 1}) );
  REQUIRE( tmt::rankToPosition(2, 3) == tmt::Position({2, 0}) );
}

TEST_CASE( "Position to Rank.", "[helpers]" ) {
  REQUIRE_THROWS( tmt::positionToRank({0, 0}, 0) ); // zero-width
  REQUIRE_THROWS( tmt::positionToRank({6, 5}, 5) );
  REQUIRE( tmt::positionToRank({0, 1}, 5) == 5 );
  REQUIRE( tmt::positionToRank({2, 0}, 5) == 2 );
  REQUIRE( tmt::positionToRank({2, 2}, 5) == 12 );
}
