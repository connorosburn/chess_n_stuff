#include "Checkers.hpp"
#include "../../json.hpp"
#include <algorithm>

struct CheckersPosition {
    CheckersPosition(int X, int Y): x(X), y(Y) {}
    CheckersPosition(nlohmann::json serializedPosition):
    x(serializedPosition["x"]), y(serializedPosition["y"]) {}
    bool operator==(const CheckersPosition& pos) const {
        return pos.x == x && pos.y == y;
    }

    CheckersPosition operator+(const CheckersPosition& pos) const {
        return CheckersPosition(x + pos.x, y + pos.y);
    }

    CheckersPosition operator-(const CheckersPosition& pos) const {
        return CheckersPosition(x - pos.x, y - pos.y);
    }

    CheckersPosition operator*(int scale) const {
        return CheckersPosition(x * scale, y * scale);
    }

    CheckersPosition operator/(int scale) const {
        return CheckersPosition(x / scale, y / scale);
    }

    bool onBoard() const {
        return y >= 0 && x >= 0 && y < checkersBoardSize && x < checkersBoardSize;
    }
    nlohmann::json serialize() const {
        nlohmann::json j;
        j["x"] = x;
        j["y"] = y;
        return j;
    }
    int x;
    int y;
};

std::vector<CheckersPosition> movementDirections(const CheckersPiece& piece) {
    std::vector<CheckersPosition> directions;
    if(piece.getType() == CheckersType::King) {
        directions = {CheckersPosition(1, 1), CheckersPosition(1, -1), CheckersPosition(-1, 1), CheckersPosition(-1, -1)};
    } else if(piece.getPlayer() == Player::White) {
        directions = {CheckersPosition(1, -1), CheckersPosition(-1, -1)};
    } else if(piece.getPlayer() == Player::Black) {
        directions = {CheckersPosition(1, 1), CheckersPosition(-1, 1)};
    }
    return directions;
}

bool jumpAvailable(const std::vector<std::vector<CheckersPiece>>& board, const CheckersPosition& start, const CheckersPosition& direction) {
    const CheckersPosition enemyPosition { start + direction };
    const CheckersPosition targetPosition { start + (direction * 2) };
    bool available {
        start.onBoard() && enemyPosition.onBoard() && targetPosition.onBoard()
    };
    if(available) {
        const CheckersPiece currentTile { board[start.y][start.x] };
        const CheckersPiece enemyTile { board[enemyPosition.y][enemyPosition.x] };
        const CheckersPiece targetTile { board[targetPosition.y][targetPosition.x] };
        available = enemyTile.getPlayer() != currentTile.getPlayer() && 
            enemyTile.getPlayer() != Player::Null &&
            targetTile.getPlayer() == Player::Null;
    }
    return available;
}

bool kingEligible(const CheckersPiece& piece, const CheckersPosition& destination) {
    return piece.getType() == CheckersType::Soldier &&
        (destination.y == 0 || destination.y == checkersBoardSize - 1);
}

std::vector<Game*> Checkers::everyJumpOutcomeFrom(int x, int y, std::vector<std::vector<CheckersPiece>> runningBoard) {
    const std::vector<CheckersPosition> directions { movementDirections(runningBoard[y][x]) };
    const CheckersPosition currentPosition(x, y);
    std::vector<Game*> outcomes;
    for(const CheckersPosition& direction : directions) {
        if(jumpAvailable(runningBoard, currentPosition, direction)) {
            const CheckersPosition attackTile { currentPosition + direction };
            const CheckersPosition destinationTile { currentPosition + (direction * 2) };
            std::vector<std::vector<CheckersPiece>> copyBoard { runningBoard };
            copyBoard[destinationTile.y][destinationTile.x] = copyBoard[currentPosition.y][currentPosition.x];
            copyBoard[currentPosition.y][currentPosition.x] = CheckersPiece();
            copyBoard[attackTile.y][attackTile.x] = CheckersPiece();
            if(kingEligible(copyBoard[destinationTile.y][destinationTile.x], destinationTile)) {
                copyBoard[destinationTile.y][destinationTile.x].makeKing();
                outcomes.emplace_back(new Checkers(copyBoard, turnCount + 1));
            } else {
                std::vector<Game*> childOutcomes { everyJumpOutcomeFrom(destinationTile.x, destinationTile.y, copyBoard) };
                outcomes.insert(outcomes.end(), childOutcomes.begin(), childOutcomes.end());
            }
        }
    }
    if(outcomes.empty()) {
        outcomes.emplace_back(new Checkers(runningBoard, turnCount + 1));
    }
    return outcomes;
}

std::vector<Game*> Checkers::everyOutcomeFrom(int x, int y) {
    std::vector<Game*> games;
    const std::vector<CheckersPosition> directions { movementDirections(board[y][x]) };
    const CheckersPosition currentPosition(x, y);
    for(const CheckersPosition& direction : directions) {
        const CheckersPosition standardMove = direction + currentPosition;
        if(standardMove.onBoard() && board[standardMove.y][standardMove.x].getPlayer() == Player::Null) {
            std::vector<std::vector<CheckersPiece>> copyBoard = board;
            copyBoard[standardMove.y][standardMove.x] = copyBoard[y][x];
            copyBoard[y][x] = CheckersPiece();
            if(kingEligible(copyBoard[standardMove.y][standardMove.x], standardMove)) {
                copyBoard[standardMove.y][standardMove.x].makeKing();
            }
            games.emplace_back(new Checkers(copyBoard, turnCount + 1));
        } else if(jumpAvailable(board, currentPosition, direction)) {
            const CheckersPosition attackTile { currentPosition + direction };
            const CheckersPosition destinationTile { currentPosition + (direction * 2) };
            std::vector<std::vector<CheckersPiece>> copyBoard  = board;
            copyBoard[destinationTile.y][destinationTile.x] = copyBoard[currentPosition.y][currentPosition.x];
            copyBoard[currentPosition.y][currentPosition.x] = CheckersPiece();
            copyBoard[attackTile.y][attackTile.x] = CheckersPiece();
            if(!kingEligible(copyBoard[destinationTile.y][destinationTile.x], destinationTile)) {
                std::vector<Game*> jumpOutcomes { everyJumpOutcomeFrom(destinationTile.x, destinationTile.y, copyBoard) };
                if(jumpOutcomes.empty()) {
                    games.emplace_back(new Checkers(copyBoard, turnCount + 1));
                } else {
                    games.insert(games.end(), jumpOutcomes.begin(), jumpOutcomes.end());
                }
            } else {
                copyBoard[destinationTile.y][destinationTile.x].makeKing();
                games.emplace_back(new Checkers(copyBoard, turnCount + 1));
            }
        }
    }
    return games;
}

std::vector<Game*> Checkers::everyHypotheticalGame() {
    std::vector<Game*> games;
    const Player currentPlayer = playerTurn();
    for(int y = 0; y < checkersBoardSize; y++) {
        for(int x = 0; x < checkersBoardSize; x++) {
            if(board[y][x].getPlayer() == currentPlayer) {
                std::vector<Game*> newGames { everyOutcomeFrom(x, y) };
                games.insert(games.end(), newGames.begin(), newGames.end());
            }
        }
    }
    return games;
}


//distinct from everyJumpOutcome for performance/minmax
nlohmann::json everySerializedJumpFrom(int x, int y, std::vector<std::vector<CheckersPiece>> runningBoard) {
    const std::vector<CheckersPosition> directions { movementDirections(runningBoard[y][x]) };
    const CheckersPosition currentPosition(x, y);
    nlohmann::json moves = nlohmann::json::array();
    for(const CheckersPosition& direction : directions) {
        if(jumpAvailable(runningBoard, currentPosition, direction)) {
            const CheckersPosition attackPosition { currentPosition + direction };
            const CheckersPosition destinationPosition { currentPosition + (direction * 2) };
            nlohmann::json move;
            move["type"] = "jump";
            move["currentPosition"] = currentPosition.serialize();
            move["attackPosition"] = attackPosition.serialize();
            move["destinationPosition"] = destinationPosition.serialize();
            std::vector<std::vector<CheckersPiece>> copyBoard { runningBoard };
            copyBoard[destinationPosition.y][destinationPosition.x] = copyBoard[currentPosition.y][currentPosition.x];
            copyBoard[currentPosition.y][currentPosition.x] = CheckersPiece();
            copyBoard[attackPosition.y][attackPosition.x] = CheckersPiece();
            move["childJumps"] = nlohmann::json::array();
            if(!kingEligible(copyBoard[destinationPosition.y][destinationPosition.x], destinationPosition)) {
                move["childJumps"] = everySerializedJumpFrom(destinationPosition.x, destinationPosition.y, copyBoard);
            }
            moves.push_back(move);
        }
    }
    return moves;
}

nlohmann::json Checkers::everySerializedMoveFrom(int x, int y) {
    nlohmann::json moves = nlohmann::json::array();
    const std::vector<CheckersPosition> directions { movementDirections(board[y][x]) };
    const CheckersPosition currentPosition(x, y);
    for(const CheckersPosition& direction : directions) {
        const CheckersPosition standardMove = direction + currentPosition;
        if(standardMove.onBoard() && board[standardMove.y][standardMove.x].getPlayer() == Player::Null) {
            nlohmann::json move;
            move["type"] = "standard";
            move["start"] = currentPosition.serialize();
            move["end"] = standardMove.serialize();
            moves.push_back(move);
        } else if(jumpAvailable(board, currentPosition, direction)) {
            const CheckersPosition attackPosition { currentPosition + direction };
            const CheckersPosition destinationPosition { currentPosition + (direction * 2) };
            nlohmann::json move;
            move["type"] = "jump";
            move["currentPosition"] = currentPosition.serialize();
            move["attackPosition"] = attackPosition.serialize();
            move["destinationPosition"] = destinationPosition.serialize();
            std::vector<std::vector<CheckersPiece>> copyBoard { board };
            copyBoard[destinationPosition.y][destinationPosition.x] = copyBoard[currentPosition.y][currentPosition.x];
            copyBoard[currentPosition.y][currentPosition.x] = CheckersPiece();
            copyBoard[attackPosition.y][attackPosition.x] = CheckersPiece();
            const bool kingEligible { 
                copyBoard[destinationPosition.y][destinationPosition.x].getType() == CheckersType::Soldier &&
                (destinationPosition.y == 0 || destinationPosition.y == checkersBoardSize - 1)
            };
            move["childJumps"] = nlohmann::json::array();
            if(!kingEligible) {
                move["childJumps"] = everySerializedJumpFrom(destinationPosition.x, destinationPosition.y, copyBoard);
            }
            moves.push_back(move);
        }
    }
    return moves;
}

std::string Checkers::everyLegalMove() {
    nlohmann::json moves = nlohmann::json::array();
    const Player currentPlayer = playerTurn();
    for(int y = 0; y < checkersBoardSize; y++) {
        for(int x = 0; x < checkersBoardSize; x++) {
            if(board[y][x].getPlayer() == currentPlayer) {
                nlohmann::json j = everySerializedMoveFrom(x, y);
                moves.insert(moves.end(), j.begin(), j.end());
            }
        }
    }
    return moves.dump();
}


//early return important for performance, gets used in minmax
bool Checkers::over() {
    const Player currentPlayer = playerTurn();
    for(int y = 0; y < checkersBoardSize; y++) {
        for(int x = 0; x < checkersBoardSize; x++) {
            const CheckersPosition currentPosition(x, y);
            if(board[y][x].getPlayer() == currentPlayer) {
                const std::vector<CheckersPosition> directions { movementDirections(board[y][x]) };
                for(const auto& direction : directions) {
                    CheckersPosition standardMove = currentPosition + direction;
                    const bool standardOpen {
                        standardMove.onBoard() && board[standardMove.y][standardMove.x].getPlayer() == Player::Null
                    };
                    if(standardOpen || jumpAvailable(board, currentPosition, direction)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Checkers::move(std::string gameMove) {
    nlohmann::json parsedMove = nlohmann::json::parse(gameMove);
    bool valid { !parsedMove["path"].empty() };
    const CheckersPosition start(parsedMove["start"]);
    const CheckersPosition first(parsedMove["path"].front());
    const std::vector<CheckersPosition> directions { movementDirections(board[start.y][start.x]) };
    const bool standardMove {
        parsedMove["path"].size() == 1 && 
        std::abs(first.x - start.x) == 1 &&
        std::abs(first.y - start.y) == 1 && 
        first.onBoard() && 
        board[first.y][first.x].getPlayer() == Player::Null &&
        std::any_of(directions.begin(), directions.end(), [&start, &first](CheckersPosition p) {
            return (first - start) == p;
        })
    };
    if(standardMove) {
        board[first.y][first.x] = board[start.y][start.x];
        board[start.y][start.x] = CheckersPiece();
        valid = true;
        if(kingEligible(board[first.y][first.x], first)) {
            board[first.y][first.x].makeKing();
        }
        turnCount++;
    } else {
        std::vector<std::vector<CheckersPiece>> runningBoard { board };
        CheckersPosition lastPosition { start };
        for(int i = 0; i < parsedMove["path"].size() && valid; i++) {
            const CheckersPosition position(parsedMove["path"][i]);
            const CheckersPosition direction { (position - lastPosition) / 2 };
            const bool jumpValid {
                jumpAvailable(runningBoard, lastPosition, direction) &&
                std::any_of(directions.begin(), directions.end(), [&direction](CheckersPosition p) {
                    return direction == p;
                })
            };
            valid = valid && jumpValid;
            if(kingEligible(runningBoard[lastPosition.y][lastPosition.x], position)) {
                runningBoard[lastPosition.y][lastPosition.x].makeKing();
                valid = valid && i == parsedMove["path"].size() - 1;
            }
            if(valid) {
                const CheckersPosition attackPosition {lastPosition + direction};
                runningBoard[position.y][position.x] = runningBoard[lastPosition.y][lastPosition.x];
                runningBoard[lastPosition.y][lastPosition.x] = CheckersPiece();
                runningBoard[attackPosition.y][attackPosition.x] = CheckersPiece();
            }
            lastPosition = position;
        }
        if(valid) {
            board = runningBoard;
            turnCount++;
        }
    }
    return valid;
}