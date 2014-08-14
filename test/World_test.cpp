#include "ext/catch.hpp"
#include "Board.hpp"
#include "Config.hpp"
#include "Piece.hpp"
#include "World.hpp"
#include "test_helpers.hpp"
#include "fixtures/ConfigFixt.hpp"

#include <cstdlib>

TEST_CASE( "World is a singleton", "[world]" ) {
  World& w1 = World::getInstance();
  World& w2 = World::getInstance();
  REQUIRE( &w1 == &w2 );
}

TEST_CASE( "World logic", "[world]" ) {
  tmt::LogCapture logCap;
  std::shared_ptr<Config> conf = std::make_shared<Config>(confFixt1());
  World& w1 = World::getInstance();
  w1.populate(conf);

  SECTION( "World setup" ) {
    REQUIRE( w1.getWoodSpecies("pine") );
    REQUIRE( w1.getWoodSpecies("cypress") );
    REQUIRE( !w1.getWoodSpecies("NOWSPECIES") );
    REQUIRE( w1.getWoodSpeciesSize() == 5 );
    REQUIRE( w1.getTermiteSpecies("brown") );
    REQUIRE( w1.getTermiteSpecies("red") );
    REQUIRE( !w1.getWoodSpecies("NOTSPECIES") );
    REQUIRE( w1.getTermiteSpeciesSize() == 3 );
    auto brd = R"( 0123456
0-------
1-T-----
2--ww---
3-------
4-----T-
5-----w-
6------w
7------w)";
    REQUIRE( w1.board.dump() == brd );
    REQUIRE( !w1.board({0,0}) );
    PiecePtr p1 = w1.board({1,1});
    REQUIRE( p1 );
    REQUIRE( p1->isTermite() );
    REQUIRE( !p1->isWoodChip() );
    PiecePtr p2 = w1.board({2,2});
    REQUIRE( p2 );
    REQUIRE( !p2->isTermite() );
    REQUIRE( p2->isWoodChip() );
    REQUIRE( w1.board({6,7}) );
  }

  SECTION( "World tic" ) {
    w1.tic();
  }

}
