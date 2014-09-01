#include "ext/catch.hpp"
#include "test_helpers.hpp"
#include "fixtures/ConfigFixt.hpp"
#include "Board.hpp"
#include "Config.hpp"
#include "Piece.hpp"
#include "Game.hpp"
#include "Termite.hpp"
#include "WoodChip.hpp"

TEST_CASE( "Game logic", "[game]" ) {
  tmt::LogCapture logCap;
  std::shared_ptr<Config> conf = std::make_shared<Config>(confFixt1());
  Game g1;
  g1.init(conf);

  SECTION( "Game setup" ) {
    REQUIRE( g1.getWoodSpecies("pine") );
    REQUIRE( g1.getWoodSpecies("cypress") );
    REQUIRE( !g1.getWoodSpecies("NOWSPECIES") );
    REQUIRE( g1.getWoodSpeciesList().size() == 5 );
    REQUIRE( g1.getTermiteSpecies("brown") );
    REQUIRE( g1.getTermiteSpecies("red") );
    REQUIRE( !g1.getWoodSpecies("NOTSPECIES") );
    REQUIRE( g1.getTermiteSpeciesList().size() == 3 );
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

TEST_CASE( "Termite move", "[game]" ) {
  tmt::LogCapture logCap;
  Game g1;
  g1.board.resize({3,3});
  WoodSpeciesPtr wspc1(new WoodSpecies("oak"));
  TermiteSpeciesPtr tspc1(new TermiteSpecies("brown", {wspc1}));
  g1.board({1,1}) = PiecePtr(new Termite(tspc1));

  g1.runTermite({1,1});
  REQUIRE( !g1.board({1,1}) );
  tmt::Position* pos = nullptr;
  std::vector<tmt::Position> dirs =
    {{0,0},{1,0},{2,0}, {0,1},{2,1}, {0,2},{1,2},{2,2}};
  for (auto p : dirs) {
    if (g1.board(p) && g1.board(p)->isTermite()) {
      pos = new tmt::Position(p);
      break;
    }
  }
  REQUIRE( pos );
  delete pos;
}

TEST_CASE( "Termite un-/load", "[game]" ) {
  tmt::LogCapture logCap;
  Game g1;
  g1.board.resize({2,2});
  WoodSpeciesPtr wspc1(new WoodSpecies("oak"));
  TermiteSpeciesPtr tspc1(new TermiteSpecies("brown", {wspc1}));

  SECTION( "must load and drop" ) {
    g1.board({0,0}) = PiecePtr(new Termite(tspc1));
    g1.board({1,0}) = PiecePtr(new WoodChip(wspc1));
    g1.board({1,1}) = PiecePtr(new WoodChip(wspc1));
    REQUIRE( !(std::dynamic_pointer_cast<Termite>(g1.board({0,0})))->isLoaded() );

    g1.runTermite({0,0});
    std::vector<tmt::Position> termitePositions = g1.board.getTermitePositions();
    REQUIRE( std::dynamic_pointer_cast<Termite>(
               g1.board(termitePositions[0]))->isLoaded() );

    g1.runTermite(termitePositions[0]); // must drop
    termitePositions = g1.board.getTermitePositions();
    REQUIRE( !std::dynamic_pointer_cast<Termite>(
               g1.board(termitePositions[0])
               )->isLoaded() );
  }

  SECTION( "must not load and remain unloaded" ) {
    WoodSpeciesPtr wspc2(new WoodSpecies("pine"));
    g1.board({0,0}) = PiecePtr(new Termite(tspc1));
    g1.board({1,0}) = PiecePtr(new WoodChip(wspc2));
    g1.board({1,1}) = PiecePtr(new WoodChip(wspc2));
    REQUIRE( !std::dynamic_pointer_cast<Termite>(g1.board({0,0}))->isLoaded() );

    g1.runTermite({0,0});
    std::vector<tmt::Position> termitePositions = g1.board.getTermitePositions();
    REQUIRE( !std::dynamic_pointer_cast<Termite>(
                 g1.board(termitePositions[0]))->isLoaded() );

    g1.runTermite(termitePositions[0]); // must remain unloaded
    termitePositions = g1.board.getTermitePositions();
    REQUIRE( !std::dynamic_pointer_cast<Termite>(
               g1.board(termitePositions[0])
               )->isLoaded() );
  }

  SECTION( "must load and remain loaded" ) {
    WoodSpeciesPtr wspc2(new WoodSpecies("pine"));
    g1.board({0,0}) = PiecePtr(new Termite(tspc1));
    g1.board({1,0}) = PiecePtr(new WoodChip(wspc1));
    g1.board({1,1}) = PiecePtr(new WoodChip(wspc2));
    REQUIRE( !std::dynamic_pointer_cast<Termite>(g1.board({0,0}))->isLoaded() );

    g1.runTermite({0,0});
    std::vector<tmt::Position> termitePositions = g1.board.getTermitePositions();
    REQUIRE( std::dynamic_pointer_cast<Termite>(
                 g1.board(termitePositions[0]))->isLoaded() );

    g1.runTermite(termitePositions[0]); // must remain loaded
    termitePositions = g1.board.getTermitePositions();
    REQUIRE( std::dynamic_pointer_cast<Termite>(
               g1.board(termitePositions[0])
               )->isLoaded() );
  }
}
