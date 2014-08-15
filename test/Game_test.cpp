#include "ext/catch.hpp"
#include "Board.hpp"
#include "Config.hpp"
#include "Piece.hpp"
#include "Game.hpp"
#include "test_helpers.hpp"
#include "fixtures/ConfigFixt.hpp"

TEST_CASE( "Game is a singleton", "[game]" ) {
  Game& g1 = Game::getInstance();
  Game& g2 = Game::getInstance();
  REQUIRE( &g1 == &g2 );
}

TEST_CASE( "Game logic", "[game]" ) {
  tmt::LogCapture logCap;
  std::shared_ptr<Config> conf = std::make_shared<Config>(confFixt1());
  Game& g1 = Game::getInstance();
  g1.populate(conf);

  SECTION( "Game setup" ) {
    REQUIRE( g1.getWoodSpecies("pine") );
    REQUIRE( g1.getWoodSpecies("cypress") );
    REQUIRE( !g1.getWoodSpecies("NOWSPECIES") );
    REQUIRE( g1.getWoodSpeciesSize() == 5 );
    REQUIRE( g1.getTermiteSpecies("brown") );
    REQUIRE( g1.getTermiteSpecies("red") );
    REQUIRE( !g1.getWoodSpecies("NOTSPECIES") );
    REQUIRE( g1.getTermiteSpeciesSize() == 3 );
    auto brd = R"( 0123456
0-------
1-T-----
2--ww---
3-------
4-----T-
5-----w-
6------w
7------w)";
    REQUIRE( g1.board.dump() == brd );
    REQUIRE( !g1.board({0,0}) );
    PiecePtr p1 = g1.board({1,1});
    REQUIRE( p1 );
    REQUIRE( p1->isTermite() );
    REQUIRE( !p1->isWoodChip() );
    PiecePtr p2 = g1.board({2,2});
    REQUIRE( p2 );
    REQUIRE( !p2->isTermite() );
    REQUIRE( p2->isWoodChip() );
    REQUIRE( g1.board({6,7}) );
  }

  SECTION( "Game tic" ) {
    g1.tic();
  }

}
