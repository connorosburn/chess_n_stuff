#include "../catch.hpp"
#include "../../lib/GameEngines/TicTacToe/TicTacToe.hpp"

TEST_CASE("It allows a legal move") {
    TicTacToe ttt;

    REQUIRE(ttt.move(0, 0));
}

TEST_CASE("It doesnt allow an illegal move") {
    TicTacToe ttt;

    REQUIRE(ttt.move(0, 0));
    REQUIRE(!ttt.move(0, 0));
}

TEST_CASE("It recognizes vertical victory") {
    TicTacToe ttt;

    REQUIRE(ttt.move(0, 0));
    REQUIRE(!ttt.move(0, 0));
}