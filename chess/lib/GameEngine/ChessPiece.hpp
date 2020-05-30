#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include "../json.hpp"
using namespace nlohmann;

#include <string>

class ChessPiece {
    public:
        ChessPiece(char pieceType, int playerNumber);
        ChessPiece();
        ChessPiece(json j);
        json serialize();
        std::string toString();
        bool isNull();
        bool enPassant();
        int getPlayer();
        char getType();
        bool hasMoved();
        void move();
    private:
        int player;
        char type;
        bool null;
        bool enpassant;
        bool moved;
        bool pieceValid(char pieceType, int playerNumber);
};

#endif