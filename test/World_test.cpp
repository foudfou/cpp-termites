#include "ext/catch.hpp"
#include "World.hpp"

TEST_CASE( "World is a singleton", "[world]" ) {
  World& w1 = World::getInstance();
  World& w2 = World::getInstance();
  REQUIRE( &w1 == &w2 );
}
