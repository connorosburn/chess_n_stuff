#include "Checkers.hpp"
#include "../../json.hpp"

Checkers::Checkers(): board(defaultCheckersBoard), turnCount(0) {

}

Checkers::Checkers(std::string snapshot) {
    nlohmann::json parsedSnapshot = nlohmann::json::parse(snapshot);
    turnCount = parsedSnapshot["turnCount"];
    for(const auto& row : parsedSnapshot["board"]) {
        board.emplace_back();
        for(const auto& tile : row) {
            board.back().emplace_back(tile);
        }
    }
}

Checkers::Checkers(std::vector<std::vector<CheckersPiece>> newBoard, int turns):
 board(newBoard), turnCount(turns) {

}

int Checkers::getTurnCount() {
    return turnCount;
}

std::string Checkers::getSnapshot() {
    nlohmann::json snapshot;
    snapshot["turnCount"] = turnCount;
    snapshot["board"] = nlohmann::json::array();
    for(const auto& row : board) {
        snapshot["board"].push_back(nlohmann::json::array());
        for(const auto& tile : row) {
            snapshot["board"].back().emplace_back(tile.getSnapshot());
        }
    }
    return snapshot.dump();
}

std::string Checkers::endState() {
    std::string state;
    if(over()) {
        Player winner { Player::Null };
        for(const auto& row : board) {
            for(const auto& tile : row) {
                if(winner == Player::Null && tile.getPlayer() != Player::Null) {
                    winner = tile.getPlayer();
                } else if(winner != Player::Null && tile.getPlayer() != Player::Null && tile.getPlayer() != winner) {
                    nlohmann::json j;
                    j["winner"] = "draw";
                    return j.dump();
                }
            }

            nlohmann::json j;
            j["winner"] = playerString.at(winner);
            state = j.dump();
        }
    }
    return state;
}

double Checkers::getScore(Player player) {
    std::map<Player, int> scores {{
        {Player::White, 0},
        {Player::Black, 0}
    }};
    for(const auto& row : board) {
        for(const auto& tile : row) {
            const Player currentPlayer = tile.getPlayer();
            if(currentPlayer != Player::Null) {
                switch(tile.getType()) {
                    case CheckersType::Soldier:
                        scores[currentPlayer] += 1;
                        break;
                    case CheckersType::King:
                        scores[currentPlayer]+= 2;
                        break;
                    case CheckersType::Null:
                        break;
                }
            }
        }
    }
    int totalScore = scores[Player::White] + scores[Player::Black];
    return static_cast<double>(scores[player]) / static_cast<double>(totalScore);
}


