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
        int getPlayer();
        bool hasMoved();
        void move();
    private:
        int player;
        char type;
        bool null;
        bool moved;
        bool pieceValid(char pieceType, int playerNumber);
};

#endif