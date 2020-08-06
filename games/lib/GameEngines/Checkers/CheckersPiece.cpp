#include "CheckersPiece.hpp"

CheckersPiece::CheckersPiece(): type(CheckersType::Null), player(Player::Null) {

}

CheckersPiece::CheckersPiece(Player playerColor): type(CheckersType::Soldier), player(playerColor) {

}

CheckersPiece::CheckersPiece(nlohmann::json snapshot) {
    type = checkersTypeFromString(snapshot["type"]);
    player = Game::playerFromString(snapshot["player"]);
}

nlohmann::json CheckersPiece::getSnapshot() const {
    nlohmann::json j;
    j["type"] = checkersTypeString.at(type);
    j["player"] = Game::playerString.at(player);
    return j;
}

std::map<CheckersType, std::string> CheckersPiece::checkersTypeString {{
    {CheckersType::Soldier, "soldier"},
    {CheckersType::King, "king"},
    {CheckersType::Null, "null"}
}};

CheckersType CheckersPiece::checkersTypeFromString(std::string typeString) {
    CheckersType checkersType { CheckersType::Null };
    for(const auto& [type, raw] : checkersTypeString) {
        if(raw == typeString) {
            checkersType = type;
        }
    }
    return checkersType;
}

Player CheckersPiece::getPlayer() const {
    return player;
}

CheckersType CheckersPiece::getType() const {
    return type;
}

void CheckersPiece::makeKing() {
    type = CheckersType::King;
}