#include "../catch.hpp"
#include "../../lib/GameEngines/Chess/Chess.hpp"

TEST_CASE("The game starts with zero moves") {
    Chess chess;
    REQUIRE(chess.getTurnCount() == 0);
}

TEST_CASE("Can tell you where pieces are") {
    Chess chess;
    REQUIRE(chess.getPiece("a2").getType() == PieceType::Pawn);
    REQUIRE(chess.getPiece("a2").getPlayer() == Player::White);
    REQUIRE(chess.getPiece("a1").getType() == PieceType::Rook);
    REQUIRE(chess.getPiece("a1").getPlayer() == Player::White);
    REQUIRE(chess.getPiece("a7").getType() == PieceType::Pawn);
    REQUIRE(chess.getPiece("a7").getPlayer() == Player::Black);
    REQUIRE(chess.getPiece("a8").getType() == PieceType::Rook);
    REQUIRE(chess.getPiece("a8").getPlayer() == Player::Black);
    REQUIRE(chess.getPiece("c8").getType() == PieceType::Bishop);
    REQUIRE(chess.getPiece("c8").getPlayer() == Player::Black);
    REQUIRE(chess.getPiece("d1").getType() == PieceType::Queen);
    REQUIRE(chess.getPiece("d1").getPlayer() == Player::White);
}

TEST_CASE("Increments the turn counter") {
    Chess chess;
    REQUIRE(chess.getTurnCount() == 0);
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.getTurnCount() == 1);
    REQUIRE(chess.move("c7", "c5"));
    REQUIRE(chess.getTurnCount() == 2);
}

TEST_CASE("Does not allow a player to move out of turn") {
    Chess chess;
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(!chess.move("b2", "b4"));
}

TEST_CASE("Allows a pawn to move twice on its first move") {
    Chess chess;
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.move("c7", "c5"));
    REQUIRE(chess.getPiece("a4").getType() == PieceType::Pawn);
    REQUIRE(chess.getPiece("a4").getPlayer() == Player::White);
    REQUIRE(chess.getPiece("c5").getType() == PieceType::Pawn);
    REQUIRE(chess.getPiece("c5").getPlayer() == Player::Black);
}

TEST_CASE("Does not allow a pawn to move twice after the first move") {
    Chess chess;
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.move("c7", "c5"));
    REQUIRE(!chess.move("a4", "a6"));
}

TEST_CASE("Does not allow a pawn to move diagonally under normal circumstances") {
    Chess chess;
    REQUIRE(!chess.move("a2", "b3"));
}

TEST_CASE("Allows a pawn to capture") {
    Chess chess;
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.move("b7", "b5"));
    REQUIRE(chess.move("a4", "b5"));
}

TEST_CASE("Allows rooks to move properly") {
    Chess chess;
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.move("a7", "a5"));
    REQUIRE(chess.move("a1", "a3"));
    REQUIRE(chess.move("a8", "a6"));
    REQUIRE(chess.move("a3", "c3"));
    REQUIRE(chess.move("a6", "c6"));
}

TEST_CASE("Does not allow rooks to move improperly") {
    Chess chess;
    REQUIRE(chess.move("b2", "b3"));
    REQUIRE(!chess.move("a1", "b2"));
}

TEST_CASE("Allows bishops to move properly") {
    Chess chess;
    REQUIRE(chess.move("b2", "b3"));
    REQUIRE(chess.move("d7", "d6"));
    REQUIRE(chess.move("c1", "a3"));
    REQUIRE(chess.move("c8", "h3"));
}

TEST_CASE("Does not allow bishops to move improperly") {
    Chess chess;
    REQUIRE(chess.move("c2", "c4"));
    REQUIRE(chess.move("c7", "c5"));
    REQUIRE(!chess.move("c1", "c3"));
    REQUIRE(!chess.move("c8", "c6"));
}

TEST_CASE("Allows queen to move properly") {
    Chess chess;

    REQUIRE(chess.move("d2", "d4"));
    REQUIRE(chess.move("d7", "d5"));

    REQUIRE(chess.move("d1", "d3"));
    REQUIRE(chess.move("d8", "d6"));

    REQUIRE(chess.move("d3", "a6"));
    REQUIRE(chess.move("d6", "g3"));
}

TEST_CASE("Allows knight to move properly") {
    Chess chess;

    REQUIRE(chess.move("b1", "a3"));
    REQUIRE(chess.move("g8", "h6"));
    REQUIRE(chess.move("a3", "c4"));
    REQUIRE(chess.move("h6", "f5"));
}

TEST_CASE("Allows king to move properly") {
    Chess chess;

    REQUIRE(chess.move("e2", "e3"));
    REQUIRE(chess.move("e7", "e6"));
    REQUIRE(chess.move("e1", "e2"));
    REQUIRE(chess.move("e8", "e7"));
    REQUIRE(chess.move("e2", "d3"));
    REQUIRE(chess.move("e7", "f6"));
}

const std::vector<std::vector<ChessPiece>> inCheckBoard {
    {    
        {ChessPiece(PieceType::Rook, Player::Black), ChessPiece(PieceType::Knight, Player::Black), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Bishop, Player::Black), ChessPiece(PieceType::Knight, Player::Black), ChessPiece(PieceType::Rook, Player::Black)},
        {ChessPiece(PieceType::Pawn, Player::Black), ChessPiece(PieceType::Bishop, Player::Black), ChessPiece(PieceType::Pawn, Player::Black), ChessPiece(), ChessPiece(PieceType::Queen, Player::Black), ChessPiece(PieceType::Queen, Player::White), ChessPiece(), ChessPiece()},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(PieceType::King, Player::Black)},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(PieceType::Pawn, Player::Black)},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(), ChessPiece(), ChessPiece()},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece()},
        {ChessPiece(PieceType::Pawn, Player::White), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(PieceType::Pawn, Player::White), ChessPiece()},
        {ChessPiece(PieceType::Rook, Player::White), ChessPiece(PieceType::Knight, Player::White), ChessPiece(PieceType::Bishop, Player::White), ChessPiece(), ChessPiece(PieceType::King, Player::White), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Rook, Player::White)}
    }
};

TEST_CASE("It detects check") {
    Chess chess(inCheckBoard, 0);

    REQUIRE(chess.inCheck(Player::Black));
    REQUIRE(!chess.inCheck(Player::White));

    chess = Chess();

    REQUIRE(!chess.inCheck(Player::Black));
    REQUIRE(!chess.inCheck(Player::White));
}

TEST_CASE("It wont let a player move into check") {
    Chess chess;
    REQUIRE(chess.move("a2","a3"));
    REQUIRE(chess.move("d7","d5"));
    REQUIRE(chess.move("a3","a4"));
    REQUIRE(chess.move("e7","e6"));
    REQUIRE(chess.move("a4","a5"));
    REQUIRE(chess.move("f8","b4"));
    REQUIRE(!chess.move("d2","d3"));
}

const std::vector<std::vector<ChessPiece>> checkMateBoard {
    {    
        {ChessPiece(PieceType::Rook, Player::Black), ChessPiece(PieceType::Knight, Player::Black), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Bishop, Player::Black), ChessPiece(PieceType::Knight, Player::Black), ChessPiece(PieceType::Rook, Player::Black)},
        {ChessPiece(PieceType::Pawn, Player::Black), ChessPiece(PieceType::Bishop, Player::Black), ChessPiece(PieceType::Pawn, Player::Black), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Queen, Player::White), ChessPiece(), ChessPiece()},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(PieceType::King, Player::Black)},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Queen, Player::Black), ChessPiece(PieceType::Rook, Player::White)},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(), ChessPiece(), ChessPiece()},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece()},
        {ChessPiece(PieceType::Pawn, Player::White), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(), ChessPiece(), ChessPiece(PieceType::Pawn, Player::White), ChessPiece(PieceType::Pawn, Player::White), ChessPiece()},
        {ChessPiece(PieceType::Rook, Player::White), ChessPiece(PieceType::Knight, Player::White), ChessPiece(PieceType::Bishop, Player::White), ChessPiece(), ChessPiece(PieceType::King, Player::White), ChessPiece(), ChessPiece(), ChessPiece()}
    }
};

TEST_CASE("It follows the en passant rule") {
    Chess chess;
    REQUIRE(chess.move("b2", "b3"));
    REQUIRE(chess.move("b7", "b5"));
    REQUIRE(chess.move("c2", "c3"));
    REQUIRE(chess.move("b5", "b4"));
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.move("b4", "a3"));
    REQUIRE(chess.getPiece("a4").isEmpty());
}

TEST_CASE("En Passant oppurtunity gone after one turn") {
    Chess chess;
    REQUIRE(chess.move("b2", "b3"));
    REQUIRE(chess.move("b7", "b5"));
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.move("b5", "b4"));
    REQUIRE(chess.move("c2", "c3"));
    REQUIRE(!chess.move("b4", "a3"));
}

TEST_CASE("It allows pawn promotion") {
    Chess chess;
    REQUIRE(chess.move("a2", "a4"));
    REQUIRE(chess.move("d7", "d5"));
    REQUIRE(chess.move("a4", "a5"));
    REQUIRE(chess.move("d5", "d4"));
    REQUIRE(chess.move("a5", "a6"));
    REQUIRE(chess.move("d4", "d3"));
    REQUIRE(chess.move("a6", "b7"));
    REQUIRE(chess.move("f7", "f6"));
    REQUIRE(!chess.move("b7", "a8"));
    REQUIRE(chess.move("b7", "a8", PieceType::Queen));
    REQUIRE(chess.getPiece("a8").getType() == PieceType::Queen);
    REQUIRE(chess.getPiece("a8").getPlayer() == Player::White);
}

TEST_CASE("It follows castling rules") {
    Chess chess;
    REQUIRE(chess.move("e2", "e3"));
    REQUIRE(chess.move("d7", "d5"));
    REQUIRE(chess.move("f1", "e2"));
    REQUIRE(chess.move("d8", "d6"));
    REQUIRE(chess.move("g1", "h3"));
    REQUIRE(chess.move("c8", "d7")); 
    REQUIRE(chess.move("e1", "g1")); 
    REQUIRE(chess.move("b8", "a6")); 
    REQUIRE(chess.move("a2", "a3"));
    REQUIRE(chess.move("e8", "c8")); 

    REQUIRE(chess.getPiece("g1").getType() == PieceType::King);
    REQUIRE(chess.getPiece("g1").getPlayer() == Player::White);
    REQUIRE(chess.getPiece("f1").getType() == PieceType::Rook);
    REQUIRE(chess.getPiece("f1").getPlayer() == Player::White);
    REQUIRE(chess.getPiece("c8").getType() == PieceType::King);
    REQUIRE(chess.getPiece("c8").getPlayer() == Player::Black);
    REQUIRE(chess.getPiece("d8").getType() == PieceType::Rook);
    REQUIRE(chess.getPiece("d8").getPlayer() == Player::Black);
}

TEST_CASE("It serializes and deserializes properly") {
    Chess original;
    Chess copy(original.getSnapshot());

    for(int x = 0; x < chessBoardSize; x++) {
        for(int y = 0; y < chessBoardSize; y++) {
            REQUIRE(original.getPiece(x, y).getType() == copy.getPiece(x, y).getType());
            REQUIRE(original.getPiece(x, y).getPlayer() == copy.getPiece(x, y).getPlayer());
        }
    }
}

