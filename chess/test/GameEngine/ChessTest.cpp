#include "../catch.hpp"
#include "../../lib/GameEngine/Chess.hpp"

TEST_CASE("The game starts with zero moves") {
    Chess chess;
    REQUIRE(chess.getTurnCount() == 0);
}

TEST_CASE("Can tell you where pieces are") {
    Chess chess;
    REQUIRE(chess.getPiece("a2").toString() == "p0");
    REQUIRE(chess.getPiece("a1").toString() == "r0");
    REQUIRE(chess.getPiece("a7").toString() == "p1");
    REQUIRE(chess.getPiece("a8").toString() == "r1");
    REQUIRE(chess.getPiece("c8").toString() == "b1");
    REQUIRE(chess.getPiece("d1").toString() == "q0");
}

TEST_CASE("Allows a pawn to move twice on its first move") {
    Chess chess;
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.move("c7", "c5"));
    REQUIRE(chess.getPiece("a4").toString() == "p0");
    REQUIRE(chess.getPiece("c5").toString() == "p1");
}

TEST_CASE("Does not allow a pawn to move twice after the first move") {
    Chess chess;
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.move("c7", "c5"));
    REQUIRE(!chess.move("a4", "a6"));
}