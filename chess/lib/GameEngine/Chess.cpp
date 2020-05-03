#include "Chess.hpp"

Chess::Chess(): board(defaultBoard) {

}

int Chess::getTurnCount() {
    return 0;
}

ChessPiece Chess::getPiece(std::string position) {
    return getPiece(ChessPosition(position));
}

ChessPiece Chess::getPiece(ChessPosition position) {
    return piece(position);
}

ChessPiece& Chess::piece(ChessPosition position) {
    return board[position.y][position.x];
}