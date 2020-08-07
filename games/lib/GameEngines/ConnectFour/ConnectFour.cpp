#include "ConnectFour.hpp"

ConnectFour::ConnectFour(): board(defaultConnectFourBoard) {

}

ConnectFour::ConnectFour(std::string snapshot): board(defaultConnectFourBoard) {
    nlohmann::json parsedBoard = nlohmann::json::parse(snapshot);
    for(int y = 0; y < connectFourHeight; y++) {
        for(int x = 0; x < connectFourWidth; x++) {
            board[y][x] = playerFromString(parsedBoard[y][x]);
        }
    }
}

ConnectFour::ConnectFour(ConnectFour proto, int column): ConnectFour(proto) {
    dropPiece(column);
}

std::string ConnectFour::getSnapshot() {
    nlohmann::json snapshot = nlohmann::json::array();
    for(auto row : board) {
        snapshot.push_back(nlohmann::json::array());
        for(auto tile : row) {
            snapshot.back().push_back(playerString.at(tile));
        }
    }
    return snapshot.dump();
}

bool ConnectFour::boardFull() {
    bool full { true };
    for(int i = 0; i < connectFourWidth && full; i++) {
        full = full && board.front()[i] != Player::Null;
    }
    return full;
}

std::string ConnectFour::endState() {
    std::string state;
    if(checkWinner() != Player::Null) {
        nlohmann::json j;
        j["winner"] = playerString.at(checkWinner());
        state = j.dump();
    } else if(boardFull()) {
        nlohmann::json j;
        j["winner"] = "draw";
        state = j.dump();
    }
    return state;
}

struct Coordinate {
    Coordinate(int X, int Y): x(X), y(Y) {};
    int x;
    int y;
};

//victory determined using pre-calculated const values for performance nessecary for minmax

const std::vector<std::vector<Coordinate>> victoryTracks {
    //vertical tracks
    {Coordinate(0, 0), Coordinate(0, 1), Coordinate(0, 2), Coordinate(0, 3), Coordinate(0, 4), Coordinate(0, 5)},
    {Coordinate(1, 0), Coordinate(1, 1), Coordinate(1, 2), Coordinate(1, 3), Coordinate(1, 4), Coordinate(1, 5)},
    {Coordinate(2, 0), Coordinate(2, 1), Coordinate(2, 2), Coordinate(2, 3), Coordinate(2, 4), Coordinate(2, 5)},
    {Coordinate(3, 0), Coordinate(3, 1), Coordinate(3, 2), Coordinate(3, 3), Coordinate(3, 4), Coordinate(3, 5)},
    {Coordinate(4, 0), Coordinate(4, 1), Coordinate(4, 2), Coordinate(4, 3), Coordinate(4, 4), Coordinate(4, 5)},
    {Coordinate(5, 0), Coordinate(5, 1), Coordinate(5, 2), Coordinate(5, 3), Coordinate(5, 4), Coordinate(5, 5)},
    {Coordinate(6, 0), Coordinate(6, 1), Coordinate(6, 2), Coordinate(6, 3), Coordinate(6, 4), Coordinate(6, 5)},

    //horizontal tracks
    {Coordinate(0, 0), Coordinate(1, 0), Coordinate(2, 0), Coordinate(3, 0), Coordinate(4, 0), Coordinate(5, 0), Coordinate(6, 0)},
    {Coordinate(0, 1), Coordinate(1, 1), Coordinate(2, 1), Coordinate(3, 1), Coordinate(4, 1), Coordinate(5, 1), Coordinate(6, 1)},
    {Coordinate(0, 2), Coordinate(1, 2), Coordinate(2, 2), Coordinate(3, 2), Coordinate(4, 2), Coordinate(5, 2), Coordinate(6, 2)},
    {Coordinate(0, 3), Coordinate(1, 3), Coordinate(2, 3), Coordinate(3, 3), Coordinate(4, 3), Coordinate(5, 3), Coordinate(6, 3)},
    {Coordinate(0, 4), Coordinate(1, 4), Coordinate(2, 4), Coordinate(3, 4), Coordinate(4, 4), Coordinate(5, 4), Coordinate(6, 4)},
    {Coordinate(0, 5), Coordinate(1, 5), Coordinate(2, 5), Coordinate(3, 5), Coordinate(4, 5), Coordinate(5, 5), Coordinate(6, 5)},

    //forward diagonals
    {Coordinate(0, 3), Coordinate(1, 2), Coordinate(2, 1), Coordinate(3, 0)},
    {Coordinate(0, 4), Coordinate(1, 3), Coordinate(2, 2), Coordinate(3, 1), Coordinate(4, 0)},
    {Coordinate(0, 5), Coordinate(1, 4), Coordinate(2, 3), Coordinate(3, 2), Coordinate(4, 1), Coordinate(5, 0)},
    {Coordinate(1, 5), Coordinate(2, 4), Coordinate(3, 3), Coordinate(4, 2), Coordinate(5, 1), Coordinate(6, 0)},
    {Coordinate(2, 5), Coordinate(3, 4), Coordinate(4, 3), Coordinate(5, 2), Coordinate(6, 1)},
    {Coordinate(3, 5), Coordinate(4, 4), Coordinate(5, 3), Coordinate(6, 2)},

    //backward diagonals
    {Coordinate(0, 2), Coordinate(1, 3), Coordinate(2, 4), Coordinate(3, 5)},
    {Coordinate(0, 1), Coordinate(1, 2), Coordinate(2, 3), Coordinate(3, 4), Coordinate(4, 5)},
    {Coordinate(0, 0), Coordinate(1, 1), Coordinate(2, 2), Coordinate(3, 3), Coordinate(4, 4), Coordinate(5, 5)},
    {Coordinate(1, 0), Coordinate(2, 1), Coordinate(3, 2), Coordinate(4, 3), Coordinate(5, 4), Coordinate(6, 5)},
    {Coordinate(2, 0), Coordinate(3, 1), Coordinate(4, 2), Coordinate(5, 3), Coordinate(6, 4)},
    {Coordinate(3, 0), Coordinate(4, 1), Coordinate(5, 2), Coordinate(6, 3)}
};

const int streakThreshold { 4 };

bool checkStreak(std::vector<Player>& streak, Player tile) {
    bool victory { false };
    if(streak.empty() && tile != Player::Null) {
        streak.push_back(tile);
    } else if(!streak.empty() && tile == streak.back()) {
        streak.push_back(tile);
        if(streak.size() >= streakThreshold) {
            victory = true;
        }
    } else if(!streak.empty() && tile != streak.back()) {
        streak.clear();
        if(tile != Player::Null) {
            streak.push_back(tile);
        }
    }
    return victory;
}

Player ConnectFour::checkWinner() {
    for(const auto& track : victoryTracks) {
        std::vector<Player> streak;
        for(const auto& tile : track) {
            if(checkStreak(streak, board[tile.y][tile.x])) {
                return board[tile.y][tile.x];
            }
        }
    }
    return Player::Null;
}

double ConnectFour::getScore(Player player) {
    Player winner = checkWinner();
    if(winner == player) {
        return 1;
    } else if(winner == Player::Null) {
        return 0.5;
    } else {
        return 0;
    }
}

std::vector<Game*> ConnectFour::everyHypotheticalGame() {
    std::vector<Game*> games;
    if(!over()) {
        for(int i = 0; i < connectFourWidth; i++) {
            if(board.front()[i] == Player::Null) {
                games.push_back(new ConnectFour(*this, i));
            }
        }
    }
    return games;
}

int ConnectFour::getTurnCount() {
    int count = 0;
    for(auto row : board) {
        for(auto tile : row) {
            if(tile != Player::Null) {
                count++;
            }
        }
    }
    return count;
}

void ConnectFour::dropPiece(int column) {
    int bottom = 0;
    while(bottom < connectFourHeight && board[bottom][column] == Player::Null) {
        bottom++;
    }
    board[bottom - 1][column] = playerTurn();
}

bool ConnectFour::move(int column) {
    const bool valid = board.front()[column] == Player::Null && !over();
    if(valid) {
        dropPiece(column);
    }
    return valid;
}

bool ConnectFour::move(std::string gameMove) {
    nlohmann::json parsedMove = nlohmann::json::parse(gameMove);
    return move(static_cast<int>(parsedMove["column"]));
}

std::string ConnectFour::everyLegalMove() {
    nlohmann::json j = nlohmann::json::array();
    if(!over()) {
        for(int i = 0; i < connectFourWidth; i++) {
            if(board.front()[i] == Player::Null) {
                j.push_back(i);
            }
        }
    }
    return j.dump();
}

bool ConnectFour::over() {
    return checkWinner() != Player::Null || boardFull();
}
