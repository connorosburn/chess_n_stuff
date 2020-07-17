#include "../catch.hpp"
#include "../../lib/GameEngines/Chess/ChessPosition.hpp"

TEST_CASE("It initializes from notation string") {
    ChessPosition position("a2");
    REQUIRE(position.x == 0);
    REQUIRE(position.y == 6);

    position = ChessPosition("b4");
    REQUIRE(position.x == 1);
    REQUIRE(position.y == 4);
}

TEST_CASE("It initializes from x and y") {
    ChessPosition position(0, 6);
    REQUIRE(position.x == 0);
    REQUIRE(position.y == 6);

    position = ChessPosition(1, 4);
    REQUIRE(position.x == 1);
    REQUIRE(position.y == 4);
}

TEST_CASE("It adds two positions like vectors") {
    REQUIRE(ChessPosition(2, 1) + ChessPosition(4, 8) == ChessPosition(6, 9));
    REQUIRE(ChessPosition(-1, 4) + ChessPosition(4, -4) == ChessPosition(3, 0));
}