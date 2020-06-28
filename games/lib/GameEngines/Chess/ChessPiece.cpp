#include "ChessPiece.hpp"
#include <sstream>
#include <array>
#include <algorithm>


ChessPiece::ChessPiece(PieceType pieceType, Player playerColor): type(pieceType), player(playerColor), moved(false) {

}

ChessPiece::ChessPiece(): type(PieceType::Null), player(Player::Null) {

}

ChessPiece::ChessPiece(nlohmann::json j):
type(typeFromString(j["type"])), 
player(playerFromString(j["player"])),
moved(j["moved"]) {

}

std::string typeString(PieceType type) {
    switch(type) {
        case PieceType::Pawn:
            return "pawn";
            break;
        case PieceType::Rook:
            return "rook";
            break;
        case PieceType::Knight:
            return "knight";
            break;
        case PieceType::Queen:
            return "queen";
            break;
        case PieceType::King:
            return "king";
            break;
        case PieceType::Bishop:
            return "bishop";
            break;
        case PieceType::EnPassant:
            return "en-passant";
            break;
        case PieceType::Null:
            return "null";
            break;
    }
}

nlohmann::json ChessPiece::serialize() {
    nlohmann::json jsonPiece;
    jsonPiece["type"] = typeString(type);
    jsonPiece["player"] = playerString(player);
    jsonPiece["moved"] = moved;
    return jsonPiece;
}

bool ChessPiece::isEmpty() {
    return type == PieceType::Null || type == PieceType::EnPassant;
}

Player ChessPiece::getPlayer() {
    return player;
}

PieceType ChessPiece::getType() {
    return type;
}

bool ChessPiece::hasMoved() {
    return moved;
}

void ChessPiece::move() {
    moved = true;
}

PieceType ChessPiece::typeFromString(std::string typeString) {
    if(typeString == "pawn") {
        return PieceType::Pawn;
    } else if(typeString == "rook") {
        return PieceType::Rook;
    } else if(typeString == "knight") {
        return PieceType::Knight;
    } else if(typeString == "bishop") {
        return PieceType::Bishop;
    } else if(typeString == "queen") {
        return PieceType::Queen;
    } else if(typeString == "king") {
        return PieceType::King;
    } else if(typeString == "en-passant") {
        return PieceType::EnPassant;
    } else {
        return PieceType::Null;
    }
}