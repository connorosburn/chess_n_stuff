#include "Game.hpp"

std::string playerString(Player player) {
    switch(player){
        case Player::White:
            return "white";
            break;
        case Player::Black:
            return "black";
            break;
        case Player::Null:
            return "null";
            break;
    }
}

Player playerFromString(std::string player) {
    if(player == "white") {
        return Player::White;
    } else if(player == "black") {
        return Player::Black;
    } else {
        return Player::Null;
    }
}

EndState::EndState(std::string endCondition, Player gameWinner):
condition(endCondition), winner(gameWinner) {
    
}

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