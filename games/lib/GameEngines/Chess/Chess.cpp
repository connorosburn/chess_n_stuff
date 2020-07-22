#include "Chess.hpp"

Chess::Chess(std::vector<std::vector<ChessPiece>> chessBoard, int turnNumber):
board(chessBoard), turnCount(turnNumber) {

}

Chess::Chess(): Chess(defaultBoard, 0) {

}

Chess::Chess(ChessPosition start, ChessPosition end, Chess chessGame): 
Chess(chessGame) {
    movePiece(start, end);
}

Chess::Chess(ChessPosition start, ChessPosition end, PieceType type, Chess chessGame): 
Chess(chessGame) {
    movePiece(start, end);
    piece(end) = ChessPiece(type, piece(end).getPlayer());
}

Chess::Chess(std::string jsonString) {
    using namespace nlohmann;
    json j = json::parse(jsonString);
    turnCount = j["turnCount"];
    for(auto& row : j["board"]) {
        board.emplace_back();
        for(json& jsonPiece : row) {
            board.back().emplace_back(jsonPiece);
        }
    }
}

std::string Chess::getSnapshot() {
    using namespace nlohmann;
    json j;
    j["board"] = json::array();
    j["turnCount"] = turnCount;
    for(auto row : board) {
        j["board"].push_back(json::array());
        for(auto piece : row) {
            j["board"].back().push_back(piece.serialize());
        }
    }
    return j.dump();
}

int Chess::getTurnCount() {
    return turnCount;
}

ChessPiece Chess::getPiece(std::string position) {
    return getPiece(ChessPosition(position));
}

ChessPiece Chess::getPiece(int x, int y) {
    return getPiece(ChessPosition(x, y));
}

ChessPiece Chess::getPiece(ChessPosition position) {
    return piece(position);
}

ChessPiece& Chess::piece(ChessPosition position) {
    return board[position.y][position.x];
}

std::vector<std::vector<ChessPiece>> Chess::getBoard() {
    return board;
}

int pawnDirection(Player player) {
    if(player == Player::White) {
        return -1;
    } else if(player == Player::Black) {
        return 1;
    } else {
        throw "Invalid player";
    }
}

void Chess::movePiece(ChessPosition start, ChessPosition end) {
    enPassantExceptions(start, end);
    if(isCastleAttempt(start, end)) {
        if(end.x > start.x) {
            piece(end + ChessPosition(-1, 0)) = piece({chessBoardSize - 1, start.y});
            piece({chessBoardSize - 1, start.y}) = ChessPiece();
        } else if(end.x < start.x) {
            piece(end + ChessPosition(1, 0)) = piece({0, start.y});
            piece({0, start.y}) = ChessPiece();
        }
    }
    piece(end) = piece(start);
    piece(start) = ChessPiece();
    piece(end).move();
    turnCount++;
}

void Chess::enPassantExceptions(ChessPosition start, ChessPosition end) {
    int yDirection = pawnDirection(piece(start).getPlayer());
    if(enPassant(start, end)) {
        piece({end.x, end.y - yDirection}) = ChessPiece();
    }

    for(int i = 0; i < chessBoardSize; i++) {
        std::vector<ChessPosition> enPassantTargets {
            { ChessPosition(i, 2), ChessPosition(i, 5) }
        };
        for(auto target : enPassantTargets) {
            if(piece(target).getType() == PieceType::EnPassant) {
                piece(target) = ChessPiece();
            }
        }
    }
    
    if(piece(start).getType() == PieceType::Pawn && std::abs(start.y - end.y) == 2) {
        piece({start.x, start.y + yDirection}) = ChessPiece(PieceType::EnPassant, piece(start).getPlayer());
    }
}


bool Chess::move(std::string start, std::string end) {
    return move(ChessPosition(start), ChessPosition(end));
}

bool Chess::move(std::string start, std::string end, PieceType pieceType) {
    return move(ChessPosition(start), ChessPosition(end), pieceType);
}

bool Chess::isPawnPromotion(ChessPosition start, ChessPosition end) {
    return !piece(start).isEmpty() && piece(start).getType() == PieceType::Pawn && (end.y == 0 || end.y == chessBoardSize - 1);
}

bool Chess::move(ChessPosition start, ChessPosition end) {
    bool valid { moveValid(start, end) && !isPawnPromotion(start, end) };
    if(valid) {
        movePiece(start, end);
    }
    return valid;
}

bool Chess::move(ChessPosition start, ChessPosition end, PieceType pieceType) {
    bool valid { moveValid(start, end) && isPawnPromotion(start, end) };
    const std::vector<PieceType> promotable {{PieceType::Rook, PieceType::Bishop, PieceType::Knight, PieceType::Queen}};
    valid = valid && std::any_of(promotable.begin(), promotable.end(), 
        [pieceType](PieceType t) {
            return t == pieceType;
        });
    if(valid) {
        movePiece(start, end);
        piece(end) = ChessPiece(pieceType, otherPlayer());
    }
    return valid;
}

bool Chess::move(std::string chessMove) {
    using namespace nlohmann;
    json j = json::parse(chessMove);
    ChessPosition start(j["start"]["x"], j["start"]["y"]);
    ChessPosition end(j["end"]["x"], j["end"]["y"]);
    if(j.contains("pawnPromotion")) {
        return move(start, end, ChessPiece::typeFromString(j["pawnPromotion"]));
    } else {
        return move(start, end);
    }
}

std::vector<ChessPosition> Chess::everyLegalMoveFrom(ChessPosition start) {
    std::vector<ChessPosition> moves;
    if(!piece(start).isEmpty() && playerTurn() == piece(start).getPlayer()) {
        moves = everyOpenMoveFrom(start);
        moves.erase(std::remove_if(moves.begin(), moves.end(), [this, start](ChessPosition end) {
            return this->hypotheticalCheck(start, end);
        }), moves.end());
    }
    return moves;
}

std::string Chess::everyLegalMove() {
    using namespace nlohmann;
    json jsonMoves = json::array();
    for(int y = 0; y < chessBoardSize; y++) {
        for(int x = 0; x < chessBoardSize; x++) {
            ChessPosition start(x, y);
            for(ChessPosition end : everyLegalMoveFrom(start)) {
                json jsonMove;
                jsonMove["start"] = json();
                jsonMove["start"]["x"] = start.x;
                jsonMove["start"]["y"] = start.y;
                jsonMove["end"] = json();
                jsonMove["end"]["x"] = end.x;
                jsonMove["end"]["y"] = end.y;
                jsonMoves.push_back(jsonMove);
            }
        }
    }
    return jsonMoves.dump();
}

std::vector<std::shared_ptr<Game>> Chess::everyHypotheticalGame() {
    std::vector<std::shared_ptr<Game>> games;
    for(int x = 0; x < chessBoardSize; x++) {
        for(int y = 0; y < chessBoardSize; y++) {
            ChessPosition start(x, y);
            for(auto end : everyLegalMoveFrom(start)) {
                if(isPawnPromotion(start, end)) {
                    const std::vector<PieceType> promoteable = {PieceType::Rook, PieceType::Bishop, PieceType::Knight, PieceType::Queen};
                    for(PieceType type : promoteable) {
                        std::shared_ptr<Game> newGame(new Chess(start, end, type, *this));
                        games.push_back(newGame);
                    }
                } else {
                    std::shared_ptr<Game> newGame(new Chess(start, end, *this));
                    games.push_back(newGame);
                }
            }
        }
    }
    return games;
}

bool Chess::moveValid(ChessPosition start, ChessPosition end) {
    auto potentialMoves = everyLegalMoveFrom(start);
    return std::any_of(potentialMoves.begin(), potentialMoves.end(), [end](ChessPosition position) {
        return position == end;
    });
}

std::vector<ChessPosition> Chess::everyOpenMoveFrom(ChessPosition start) {
    std::vector<ChessPosition> positions;
    switch(piece(start).getType()) {
        case PieceType::Pawn:
            positions = pawnMoves(start);
            break;
        case PieceType::Rook:
            positions = rookMoves(start);
            break;
        case PieceType::Bishop:
            positions = bishopMoves(start);
            break;
        case PieceType::Queen:
            positions = queenMoves(start);
            break;
        case PieceType::Knight:
            positions = knightMoves(start);
            break;
        case PieceType::King:
            positions = kingMoves(start);
            break;
        default:
            throw "Must move from a starting place with a piece";
            break;
    }
    return positions;
}

void Chess::pawnCaptures(ChessPosition start, std::vector<ChessPosition>& positions) {
    int yDirection { pawnDirection(piece(start).getPlayer()) };
    const std::vector<ChessPosition> captureChecks = {ChessPosition(1, yDirection), ChessPosition(-1, yDirection)};
    for(ChessPosition offset : captureChecks) {
        ChessPosition position{start + offset};
        if(position.onBoard() && !piece(position).isEmpty() && piece(position).getPlayer() != piece(start).getPlayer()) {
            positions.push_back(position);
        }
    }
}

bool Chess::enPassant(ChessPosition start, ChessPosition end) {
    int yDirection = pawnDirection(piece(start).getPlayer());
    bool valid = piece(start).getType() == PieceType::Pawn;
    valid = valid && piece(end).getType() == PieceType::EnPassant && piece(end).getPlayer() != piece(start).getPlayer();
    valid = valid && start.y + yDirection == end.y && std::abs(start.x - end.x) == 1;
    valid = valid && end.onBoard();
    return valid;
}

void Chess::enPassantPositions(ChessPosition start, std::vector<ChessPosition>& positions) {
    int yDirection { pawnDirection(piece(start).getPlayer()) };
    std::vector<ChessPosition> potentialPositions {
        { start + ChessPosition(-1, yDirection), start + ChessPosition(1, yDirection) }
    };
    for(auto& position : potentialPositions) {
        if(enPassant(start, position)) {
            positions.push_back(position);
        }
    }
}

std::vector<ChessPosition> Chess::pawnMoves(ChessPosition start) {
    int yDirection { pawnDirection(piece(start).getPlayer()) };

    std::vector<ChessPosition> positions;

    ChessPosition standardMove{ start + ChessPosition(0, yDirection) };
    ChessPosition doubleMove{ start + (ChessPosition(0, yDirection) * 2) };

    const bool standardClear { 
        standardMove.onBoard() && piece(standardMove).isEmpty() 
    };

    if(standardClear) {
        positions.push_back(standardMove);
    }

    const bool doubleClear { 
        standardClear && doubleMove.onBoard() && 
        piece(doubleMove).isEmpty() && !piece(start).hasMoved() 
    };

    if(doubleClear) {
        positions.push_back(doubleMove);
    }

    pawnCaptures(start, positions);

    enPassantPositions(start, positions);

    return positions;
}

std::vector<ChessPosition> Chess::searchAlongVectors(ChessPosition start, std::vector<ChessPosition> searchVectors) {
    std::vector<ChessPosition> positions;
    for(auto vector : searchVectors) {
        bool reachedEnd { false };
        for(ChessPosition position = start + vector; !reachedEnd; position += vector) {
            reachedEnd = !position.onBoard() || !this->piece(position).isEmpty(); 
            if(position.onBoard() && (piece(position).isEmpty() || piece(position).getPlayer() != piece(start).getPlayer())) {
                positions.push_back(position);
            }
        }
    }
    return positions;
}

std::vector<ChessPosition> Chess::checkIndividualOffsets(ChessPosition start, std::vector<ChessPosition> offsets) {
    std::vector<ChessPosition> positions;
    for(auto offset : offsets) {
        ChessPosition position = start + offset;
        if(position.onBoard() && (piece(position).isEmpty() || piece(position).getPlayer() != piece(start).getPlayer())) {
            positions.push_back(position);
        }
    }
    return positions;
}

std::vector<ChessPosition> Chess::rookMoves(ChessPosition start) {
    std::vector<ChessPosition> searchVectors {
        {
            ChessPosition(1, 0), ChessPosition(-1, 0), ChessPosition(0, 1), ChessPosition(0, -1)
        }
    };
    return searchAlongVectors(start, searchVectors);
}

std::vector<ChessPosition> Chess::bishopMoves(ChessPosition start) {
    std::vector<ChessPosition> searchVectors {
        {
            ChessPosition(1, 1), ChessPosition(-1, 1), ChessPosition(1, -1), ChessPosition(-1, -1)
        }
    };
    return searchAlongVectors(start, searchVectors);
}

std::vector<ChessPosition> Chess::queenMoves(ChessPosition start) {
    std::vector<ChessPosition> searchVectors {
        {
            ChessPosition(1, 0), ChessPosition(-1, 0), ChessPosition(0, 1), ChessPosition(0, -1),
            ChessPosition(1, 1), ChessPosition(-1, 1), ChessPosition(1, -1), ChessPosition(-1, -1)
        }
    };
    return searchAlongVectors(start, searchVectors);
}

std::vector<ChessPosition> Chess::knightMoves(ChessPosition start) {
    std::vector<ChessPosition> offsets {
        {
            ChessPosition(2, 1), ChessPosition(-2, 1), ChessPosition(2, -1), ChessPosition(-2, -1),
            ChessPosition(1, 2), ChessPosition(-1, 2), ChessPosition(1, -2), ChessPosition(-1, -2)
        }
    };
    return checkIndividualOffsets(start, offsets);
}

bool Chess::isCastleAttempt(ChessPosition start, ChessPosition end) {
    bool valid = start.onBoard() && end.onBoard();
    valid = valid && piece(start).getType() == PieceType::King && !piece(start).hasMoved();
    valid = valid && std::abs(start.x - end.x) == 2;
    if(valid) {
        ChessPosition rookPosition(0, 0);
        ChessPosition kingDirection(0, 0);
        if(end.x > start.x) {
            rookPosition = ChessPosition(chessBoardSize - 1, start.y);
            kingDirection = ChessPosition(1, 0);
        } else if(end.x < start.x) {
            rookPosition = ChessPosition(0, start.y);
            kingDirection = ChessPosition(-1, 0);
        }

        ChessPosition rookDirection = kingDirection * -1;
        valid = valid && !piece(rookPosition).isEmpty() && piece(rookPosition).getType() == PieceType::Rook && !piece(rookPosition).hasMoved();
        for(ChessPosition pos = start + kingDirection; pos != end && valid; pos += kingDirection) {
            valid = valid && piece(pos).isEmpty();
        }
        for(ChessPosition pos = rookPosition + rookDirection; pos != end + rookDirection && valid; pos += rookDirection) {
            valid = valid && piece(pos).isEmpty();
        }
    }
    return valid;
}

std::vector<ChessPosition> Chess::kingMoves(ChessPosition start) {
    std::vector<ChessPosition> offsets {
        {
            ChessPosition(1, 0), ChessPosition(-1, 0), ChessPosition(0, 1), ChessPosition(0, -1),
            ChessPosition(1, 1), ChessPosition(-1, 1), ChessPosition(1, -1), ChessPosition(-1, -1)
        }
    };
    std::vector<ChessPosition> moves { checkIndividualOffsets(start, offsets) };

    if(!piece(start).hasMoved()) {
        std::vector<ChessPosition> castleOffsets {{ChessPosition(-2, 0), ChessPosition(2, 0)}};
        for(ChessPosition offset : castleOffsets) {
            ChessPosition end = start + offset;
            if(isCastleAttempt(start, end)) {
                moves.push_back(end);
            }
        }
    }
    return moves;
}

bool Chess::inCheck(Player player) {
    bool check { false };
    for(int y = 0 ; y < chessBoardSize && !check; y++) {
        for(int x = 0; x < chessBoardSize && !check; x++) {
            if(!piece({x, y}).isEmpty() && piece({x, y}).getPlayer() != player) {
                std::vector<ChessPosition> threatened = everyOpenMoveFrom({x, y});
                check = check || std::any_of(threatened.begin(), threatened.end(), [this, player](ChessPosition position) {
                    return this->piece(position).getPlayer() == player && 
                            this->piece(position).getType() == PieceType::King;
                });
            }
        }
    }
    return check;
}

bool Chess::hypotheticalCheck(ChessPosition start, ChessPosition end) {
    auto cachedBoard = board;
    piece(end) = piece(start);
    piece(start) = ChessPiece();
    bool check = inCheck(piece(end).getPlayer());
    board = cachedBoard;
    if(isCastleAttempt(start, end)) {
        int middleX = start.x + ((end.x - start.x) / 2);
        ChessPosition middle(middleX, start.y);
        piece(middle) = piece(start);
        check = check || inCheck(piece(middle).getPlayer());
        board = cachedBoard;
    }
    return check;
}

bool Chess::playerStuck() {
    bool stuck { true };
    for(int y = 0; y < chessBoardSize && stuck; y++) {
        for(int x = 0; x < chessBoardSize && stuck; x++) {
            stuck = stuck && everyLegalMoveFrom({x, y}).empty();
        }
    }
    return stuck;
}


std::string Chess::endState() {
    using namespace nlohmann;
    std::string state;
    const bool stuck = playerStuck();
    const bool check = inCheck(playerTurn());
    if(stuck && check) {
        json j;
        j["condition"] = "checkmate";
        j["winner"] = playerString.at(otherPlayer());
        state = j.dump();
    } else if(stuck && !check) {
        json j;
        j["condition"] = "stalemate";
        state = j.dump();
    }
    return state;
}

double Chess::scorePieces(Player player) {
    const std::map<PieceType, int> values = {
        {PieceType::Pawn, 1}, 
        {PieceType::Knight, 3}, 
        {PieceType::Bishop, 3}, 
        {PieceType::Rook, 5}, 
        {PieceType::Queen, 9},
        {PieceType::King, 20}
    };

    std::map<Player, int> pieceTotals {
        {Player::White, 0}, {Player::Black, 0}
    };

    for(auto row : board) {
        for(auto piece : row) {
            if(!piece.isEmpty()) {
                pieceTotals[piece.getPlayer()] += values.at(piece.getType());
            }
        }
    }

    int totalScore = pieceTotals[Player::White] + pieceTotals[Player::Black];
    return static_cast<double>(pieceTotals[player]) / static_cast<double>(totalScore);
}

double Chess::getScore(Player player) {
    using namespace nlohmann;
    std::string rawEnd = endState();
    double score;
    if(rawEnd == "") {
        score = scorePieces(player);
    } else {
        json end = json::parse(rawEnd);
        if(end["condition"] == "checkmate" && playerString.at(player) == end["winner"]) {
            score = 1;
        } else if(end["condition"] == "checkmate" && playerString.at(player) != end["winnder"]) {
            score = 0;
        } else {
            score = 0.5;
        }
    }
    return score;
}