#include "Game.hpp"
#include <map>

Player Game::playerTurn() {
    int rawTurn = getTurnCount() % 2;
    if(rawTurn == 0) {
        return Player::White;
    } else {
        return Player::Black;
    }
}

Player Game::otherPlayer() {
    if(playerTurn() == Player::White) {
        return Player::Black;
    } else {
        return Player::White;
    }
}

const std::map<Player, std::string> Game::playerString {
    {Player::White, "white"},
    {Player::Black, "black"},
    {Player::Null, "null"}
};

Player Game::playerFromString(std::string rawPlayer) {
    Player matchedPlayer { Player::Null };
    for(const auto& [player, raw] : playerString) {
        if(raw == rawPlayer) {
            matchedPlayer = player;
        }
    }
    return matchedPlayer;
}

Game::~Game() {

}