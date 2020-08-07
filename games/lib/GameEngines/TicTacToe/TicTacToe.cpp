#include "TicTacToe.hpp"

TicTacToe::TicTacToe(): board(defaultTicTacToeBoard) {

}

TicTacToe::TicTacToe(std::string snapshot): board(defaultTicTacToeBoard) {
    nlohmann::json parsedBoard = nlohmann::json::parse(snapshot);
    for(int y = 0; y < tttBoardSize; y++) {
        for(int x = 0; x < tttBoardSize; x++) {
            board[y][x] = playerFromString(parsedBoard[y][x]);
        }
    }
}

TicTacToe::TicTacToe(TicTacToe proto, int x, int y): TicTacToe(proto) {
    board[y][x] = playerTurn();
}

double TicTacToe::getScore(Player player) {
    Player winner = checkWinner();
    if(winner == player) {
        return 1;
    } else if(winner == Player::Null) {
        return 0.5;
    } else {
        return 0;
    }
}

std::vector<Game*> TicTacToe::everyHypotheticalGame() {
    std::vector<Game*> games;
    if(checkWinner() == Player::Null) {
        for(int y = 0; y < tttBoardSize; y++) {
            for(int x = 0; x < tttBoardSize; x++) {
                if(board[y][x] == Player::Null) {
                    games.push_back(new TicTacToe(*this, x, y));
                }
            }
        }
    }
    return games;
}

bool TicTacToe::over() {
    return checkWinner() != Player::Null || boardFull();
}

int TicTacToe::getTurnCount() {
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

std::string TicTacToe::getSnapshot() {
    nlohmann::json snapshot = nlohmann::json::array();
    for(auto row : board) {
        snapshot.push_back(nlohmann::json::array());
        for(auto tile : row) {
            snapshot.back().push_back(playerString.at(tile));
        }
    }
    return snapshot.dump();
}

bool TicTacToe::move(std::string gameMove) {
    auto j = nlohmann::json::parse(gameMove);
    return move(j["x"], j["y"]);
}

bool TicTacToe::move(int x, int y) {
    bool valid = x < tttBoardSize && x >= 0 && y < tttBoardSize && y >= 0;
    valid = valid && board[y][x] == Player::Null && checkWinner() == Player::Null;
    if(valid) {
        board[y][x] = playerTurn();
    }
    return valid;
}

std::string TicTacToe::everyLegalMove() {
    nlohmann::json j = nlohmann::json::array();
    if(checkWinner() == Player::Null) {
        for(int y = 0; y < tttBoardSize; y++) {
            for(int x = 0; x < tttBoardSize; x++) {
                if(board[y][x] == Player::Null) {
                    nlohmann::json jsonMove;
                    jsonMove["x"] = x;
                    jsonMove["y"] = y;
                    j.push_back(jsonMove);
                }
            }
        }
    }
    return j.dump();
}

bool TicTacToe::boardFull() {
    bool full = true;
    for(auto row : board) {
        for(auto tile : row) {
            full = full && tile != Player::Null;
        }
    }
    return full;
}

struct Coordinate {
    Coordinate(int X, int Y): x(X), y(Y) {};
    int x;
    int y;
};

//victory determined using pre-calculated const values for performance nessecary for minmax

const std::vector<std::vector<Coordinate>> potentialWins {{
    {Coordinate(0, 0), Coordinate(1, 0), Coordinate(2, 0)},
    {Coordinate(0, 1), Coordinate(1, 1), Coordinate(2, 1)},
    {Coordinate(0, 2), Coordinate(1, 2), Coordinate(2, 2)},
    {Coordinate(0, 0), Coordinate(0, 1), Coordinate(0, 2)},
    {Coordinate(1, 0), Coordinate(1, 1), Coordinate(1, 2)},
    {Coordinate(2, 0), Coordinate(2, 1), Coordinate(2, 2)},
    {Coordinate(0, 0), Coordinate(1, 1), Coordinate(2, 2)},
    {Coordinate(2, 0), Coordinate(1, 1), Coordinate(0, 2)}
}};

Player TicTacToe::checkWinner() {
    Player winner = Player::Null;
    for(int i = 0; i < potentialWins.size() && winner == Player::Null; i++) {
        const std::vector<Coordinate>& win = potentialWins[i];
        Player contender = board[win.front().y][win.front().x];
        if(contender != Player::Null) {
            bool victory = true;
            for(const Coordinate& tile : win) {
                victory = victory && board[tile.y][tile.x] == contender;
            }
            if(victory) {
                winner = contender;
            }
        }

    }
    return winner;
}

std::string TicTacToe::endState() {
    std::string state;
    Player winner = Player::Null;
    for(int i = 0; i < potentialWins.size() && winner == Player::Null; i++) {
        const std::vector<Coordinate>& win = potentialWins[i];
        Player contender = board[win.front().y][win.front().x];
        if(contender != Player::Null) {
            bool victory = true;
            for(const Coordinate& tile : win) {
                victory = victory && board[tile.y][tile.x] == contender;
            }
            if(victory) {
                winner = contender;
                nlohmann::json j;
                j["winner"] = playerString.at(winner);
                j["tiles"] = nlohmann::json::array();
                for(const Coordinate& tile : win) {
                    j["tiles"].push_back(nlohmann::json());
                    j["tiles"].back()["x"] = tile.x;
                    j["tiles"].back()["y"] = tile.y;
                }
                state = j.dump();
            }
        }
    }
    if(state.empty() && boardFull()) {
        nlohmann::json j;
        j["winner"] = "draw";
        state = j.dump();
    }
    return state;
}