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

std::vector<std::shared_ptr<Game>> TicTacToe::everyHypotheticalGame() {
    std::vector<std::shared_ptr<Game>> games;
    if(endState().empty()) {
        for(int y = 0; y < tttBoardSize; y++) {
            for(int x = 0; x < tttBoardSize; x++) {
                if(board[y][x] == Player::Null) {
                    games.emplace_back(new TicTacToe(*this, x, y));
                }
            }
        }
    }
    return games;
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
    valid = valid && board[y][x] == Player::Null && endState().empty();
    if(valid) {
        board[y][x] = playerTurn();
    }
    return valid;
}

std::string TicTacToe::everyLegalMove() {
    nlohmann::json j = nlohmann::json::array();
    if(endState().empty()) {
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

nlohmann::json TicTacToe::checkVertical() {
    nlohmann::json winningVertical = nlohmann::json::array();
    for(int x = 0; x < tttBoardSize; x++) {
        bool victory = true;
        nlohmann::json thisVertical = nlohmann::json::array();
        for(int y = 0; y < tttBoardSize; y++) {
            nlohmann::json thisTile;
            thisTile['x'] = x;
            thisTile['y'] = y;
            thisVertical.push_back(thisTile);
            victory = victory && board[y][x] != Player::Null && board[y][x] == board[0][x];

        }
        if(victory) {
            winningVertical = thisVertical;
        }
    }
    return winningVertical;
}

nlohmann::json TicTacToe::checkHorizontal() {
    nlohmann::json winningHorizontal = nlohmann::json::array();
    for(int y = 0; y < tttBoardSize; y++) {
        bool victory = true;
        nlohmann::json thisHorizontal = nlohmann::json::array();
        for(int x = 0; x < tttBoardSize; x++) {
            nlohmann::json thisTile;
            thisTile['x'] = x;
            thisTile['y'] = y;
            thisHorizontal.push_back(thisTile);
            victory = victory && board[y][x] != Player::Null && board[y][x] == board[y][0];

        }
        if(victory) {
            winningHorizontal = thisHorizontal;
        }
    }
    return winningHorizontal;
}

nlohmann::json TicTacToe::checkDiagonal() {
    nlohmann::json winningDiagonal = nlohmann::json::array();
    nlohmann::json forwardDiagonal = nlohmann::json::array();
    nlohmann::json backwardDiagonal = nlohmann::json::array();
    bool forwardVictory = true;
    bool backwardVictory = true;
    for(int x = 0; x < tttBoardSize; x++) {
        for(int y = 0; y < tttBoardSize; y++) {
            int backX = tttBoardSize - x - 1;
            nlohmann::json fTile;
            nlohmann::json bTile;
            fTile["x"] = x;
            fTile["y"] = y;
            bTile["x"] = backX;
            bTile["y"] = y;
            forwardDiagonal.push_back(fTile);
            backwardDiagonal.push_back(bTile);
            forwardVictory = forwardVictory && board[y][x] != Player::Null && board[y][x] == board[0][0];
            backwardVictory = backwardVictory && board[y][backX] != Player::Null && board[y][backX] == board[0][tttBoardSize - 1];
        }
    }

    if(forwardVictory) {
        winningDiagonal = forwardDiagonal;
    } else if(backwardVictory) {
        winningDiagonal = backwardDiagonal;
    }
    return winningDiagonal;
}

Player TicTacToe::checkWinner() {
    std::vector<nlohmann::json> potentialWins {{checkDiagonal(), checkHorizontal(), checkVertical()}};
    Player winner = Player::Null;
    for(auto win : potentialWins) {
        if(!win.empty()) {
            winner = board[win[0]["y"]][win[0]["x"]];
        }
    }
    return winner;
}

std::string TicTacToe::endState() {
    std::string state;
    std::vector<nlohmann::json> potentialEnds { {checkVertical(), checkHorizontal(), checkDiagonal()} };
    for(nlohmann::json j : potentialEnds) {
        if(!j.empty()) {
            nlohmann::json victoryMessage;
            victoryMessage["winner"] = playerString.at(board[j[0]["y"]][j[0]["x"]]);
            victoryMessage["tiles"] = j;
            state = victoryMessage.dump();
        }
    }
    if(state.empty() && boardFull()) {
        nlohmann::json j;
        j["winner"] = "draw";
        state = j.dump();
    }
    return state;
}