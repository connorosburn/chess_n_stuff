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
player(Game::playerFromString(j["player"])),
moved(j["moved"]) {

}

nlohmann::json ChessPiece::serialize() {
    nlohmann::json jsonPiece;
    jsonPiece["type"] = ChessPiece::typeString.at(type);
    jsonPiece["player"] = Game::playerString.at(player);
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

const std::map<PieceType, std::string> ChessPiece::typeString {
    {PieceType::Pawn, "pawn"},
    {PieceType::Rook, "rook"},
    {PieceType::Knight, "knight"},
    {PieceType::Queen, "queen"},
    {PieceType::King, "king"},
    {PieceType::Bishop, "bishop"},
    {PieceType::EnPassant, "en-passant"},
    {PieceType::Null, "null"}
};

PieceType ChessPiece::typeFromString(std::string rawType) {
    PieceType matchedType { PieceType::Null };
    for(const auto& [type, raw] : typeString) {
        if(raw == rawType) {
            matchedType = type;
        }
    }
    return matchedType;
}