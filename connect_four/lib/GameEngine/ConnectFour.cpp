#include "ConnectFour.hpp"

ConnectFour::ConnectFour(): board(defaultBoard), turnCount(0) {

}

Player ConnectFour::playerTurn() {
    if(turnCount % 2 == 0) {
        return Player::One;
    } else if(turnCount % 2 == 1) {
        return Player::Two;
    }
}

Player& ConnectFour::space(int x, int y) {
    return board[y][x];
}

Player& ConnectFour::topSpace(int x) {
    return board.f
}

bool ConnectFour::move(int x) {
    bool validMove = x >= 0 && x < boardWidth && topSpace(x) == Player::Null && getWinner() == Player::Null;
    if(validMove) {
        int y {0};
        while(y >= 0 && space(x, y + 1) == Player::Null) {
            y++;
        }
        space(x, y) = playerTurn();
        turnCount++;
    }
    return validMove;
}