#include "../catch.hpp"
#include "../../lib/GameEngine/Chess.hpp"

TEST_CASE("The game starts with zero moves") {
    Chess chess;
    REQUIRE(chess.getTurnCount() == 0);
}

TEST_CASE("Can tell you where pieces are") {
    Chess chess;
    REQUIRE(chess.getPiece("a2") == "p0");
    REQUIRE(chess.getPiece("a1") == "r0");
    REQUIRE(chess.getPiece("a7") == "p1");
    REQUIRE(chess.getPiece("a8") == "r1");
    REQUIRE(chess.getPiece("c8") == "b0");
    REQUIRE(chess.getPiece("d1") == "q0");
}