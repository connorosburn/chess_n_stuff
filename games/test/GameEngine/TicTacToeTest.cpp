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
    REQUIRE(ttt.move(1, 0));
    REQUIRE(ttt.move(0, 1));
    REQUIRE(ttt.move(1, 1));

    REQUIRE(ttt.checkWinner() == Player::Null);

    REQUIRE(ttt.move(0, 2));
    REQUIRE(ttt.checkWinner() == Player::White);
}

TEST_CASE("It recognizes horizontal victory") {
    TicTacToe ttt;

    REQUIRE(ttt.move(0, 0));
    REQUIRE(ttt.move(0, 1));
    REQUIRE(ttt.move(2, 2));
    REQUIRE(ttt.move(1, 1));
    REQUIRE(ttt.move(0, 2));
    REQUIRE(ttt.checkWinner() == Player::Null);
    REQUIRE(ttt.move(2, 1));
    REQUIRE(ttt.checkWinner() == Player::Black);
}

TEST_CASE("It recognizes diagonal victory") {
    TicTacToe ttt;

    REQUIRE(ttt.move(0, 0));
    REQUIRE(ttt.move(0, 1));
    REQUIRE(ttt.move(1, 1));
    REQUIRE(ttt.move(0, 2));
    REQUIRE(ttt.checkWinner() == Player::Null);
    REQUIRE(ttt.move(2, 2));
    REQUIRE(ttt.checkWinner() == Player::White);
}