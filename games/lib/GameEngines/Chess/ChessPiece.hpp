#ifndef CHESS_PIECE_HPP
#define CHESS_PIECE_HPP

#include "../../json.hpp"
#include "../Game.hpp"

#include <string>

enum class PieceType {Pawn, Rook, Knight, Bishop, Queen, King, EnPassant, Null};

class ChessPiece {
    public:
        ChessPiece(PieceType pieceType, Player playerColor);
        ChessPiece();
        ChessPiece(nlohmann::json j);
        nlohmann::json serialize();
        Player getPlayer();
        PieceType getType();
        bool hasMoved();
        void move();
        bool isEmpty();
        static PieceType typeFromString(std::string typeString);
        std::string notationString();
    private:
        Player player;
        PieceType type;
        bool moved;
};

#endif