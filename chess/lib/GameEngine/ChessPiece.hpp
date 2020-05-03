#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include <string>

class ChessPiece {
    public:
        ChessPiece(std::string pieceType);
        ChessPiece(char pieceType, int playerNumber);
        ChessPiece();
        std::string toString();
        bool isNull();

    private:
        int player;
        char type;
        bool null;
        bool pieceValid(char pieceType, int playerNumber);
};

#endif