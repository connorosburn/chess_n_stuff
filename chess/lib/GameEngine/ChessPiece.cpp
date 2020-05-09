#include "ChessPiece.hpp"
#include <sstream>
#include <array>
#include <algorithm>

const std::array<char, 6> pieceTypes {{'p', 'r', 'n', 'b', 'q', 'k'}};

ChessPiece::ChessPiece(std::string pieceType): null(false) {

}

ChessPiece::ChessPiece(char pieceType, int playerNumber): null(false), moved(false) {
    pieceType = std::tolower(pieceType);
    if(pieceValid(pieceType, playerNumber)) {
        type = pieceType;
        player = playerNumber;
    } else {
        throw "Invalid chess piece";
    }
}

bool ChessPiece::pieceValid(char pieceType, int playerNumber) {
    bool valid = std::any_of(pieceTypes.begin(), pieceTypes.end(), 
        [pieceType](char p) { 
            return p == pieceType; 
        }
    );
    valid = valid && (playerNumber == 0 || playerNumber == 1);
    return valid;
}

ChessPiece::ChessPiece(): null(true) {

}

std::string ChessPiece::toString() {
    if(null) {
        return "";
    }
    std::stringstream outputStream;
    outputStream << type << player;
    return outputStream.str();
}

bool ChessPiece::isNull() {
    return null;
}

int ChessPiece::getPlayer() {
    return player;
}

char ChessPiece::getType() {
    return type;
}

bool ChessPiece::hasMoved() {
    return moved;
}

void ChessPiece::move() {
    moved = true;
}