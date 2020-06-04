#include "ChessPiece.hpp"
#include <sstream>
#include <array>
#include <algorithm>

const std::array<char, 6> pieceTypes {{'p', 'r', 'n', 'b', 'q', 'k'}};

ChessPiece::ChessPiece(char pieceType, int playerNumber): null(false), moved(false), enpassant(false) {
    pieceType = std::tolower(pieceType);
    if(pieceValid(pieceType, playerNumber)) {
        type = pieceType;
        player = playerNumber;
        enpassant = false;
    } else if(pieceType == 'e') {
        type = 'e';
        enpassant = true;
        null = true;
        player = playerNumber;
    } else {
        throw "Invalid chess piece";
    }
}

ChessPiece::ChessPiece(json j):
null(j["null"]),
enpassant(j["enPassant"]),
type(std::string(j["type"])[0]),
player(j["player"]),
moved(j["moved"]) {
    
}

json ChessPiece::serialize() {
    json jsonPiece;
    jsonPiece["null"] = null;
    jsonPiece["enPassant"] = enpassant;
    jsonPiece["type"] = type;
    jsonPiece["player"] = player;
    jsonPiece["moved"] = moved;
    return jsonPiece;
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

ChessPiece::ChessPiece(): null(true), enpassant(false), type('e') {

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

bool ChessPiece::enPassant() {
    return enpassant;
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