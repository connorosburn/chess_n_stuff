#include "Chess.hpp"

Chess::Chess(std::vector<std::vector<ChessPiece>> chessBoard, int turnNumber):
board(chessBoard), turnCount(turnNumber) {

}

Chess::Chess(): Chess(defaultBoard, 0) {

}

Chess::Chess(ChessPosition start, ChessPosition end, std::vector<std::vector<ChessPiece>> oldBoard, int turnNumber): 
Chess(oldBoard, turnNumber) {
    movePiece(start, end);
}

Chess::Chess(ChessPosition start, ChessPosition end, char type, std::vector<std::vector<ChessPiece>> oldBoard, int turnNumber): 
Chess(oldBoard, turnNumber) {
    movePiece(start, end);
    piece(end) = ChessPiece(type, piece(end).getPlayer());
}

int Chess::getTurnCount() {
    return turnCount;
}

int Chess::playerTurn() {
    return turnCount % 2;
}

int Chess::otherPlayer() {
    return (turnCount + 1) % 2;
}

ChessPiece Chess::getPiece(std::string position) {
    return getPiece(ChessPosition(position));
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

int pawnDirection(int player) {
    if(player == 0) {
        return -1;
    } else if(player == 1) {
        return 1;
    } else {
        throw "Invalid player";
    }
}

void Chess::movePiece(ChessPosition start, ChessPosition end) {
    enPassantExceptions(start, end);
    if(isCastleAttempt(start, end)) {
        if(end.x > start.x) {
            piece(end + ChessPosition(-1, 0)) = piece({boardSize - 1, start.y});
            piece({boardSize - 1, start.y}) = ChessPiece();
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

    for(int i = 0; i < boardSize; i++) {
        std::vector<ChessPosition> enPassantTargets {
            { ChessPosition(i, 2), ChessPosition(i, 5) }
        };
        for(auto target : enPassantTargets) {
            if(piece(target).enPassant()) {
                piece(target) = ChessPiece();
            }
        }
    }
    
    if(piece(start).getType() == 'p' && std::abs(start.y - end.y) == 2) {
        piece({start.x, start.y + yDirection}) = ChessPiece('e', piece(start).getPlayer());
    }
}


bool Chess::move(std::string start, std::string end) {
    return move(ChessPosition(start), ChessPosition(end));
}

bool Chess::move(std::string start, std::string end, char pieceType) {
    return move(ChessPosition(start), ChessPosition(end), pieceType);
}

bool Chess::isPawnPromotion(ChessPosition start, ChessPosition end) {
    return !piece(start).isNull() && piece(start).getType() == 'p' && (end.y == 0 || end.y == boardSize - 1);
}

bool Chess::move(ChessPosition start, ChessPosition end) {
    bool valid { moveValid(start, end) && !isPawnPromotion(start, end) };
    if(valid) {
        movePiece(start, end);
    }
    return valid;
}

bool Chess::move(ChessPosition start, ChessPosition end, char pieceType) {
    bool valid { moveValid(start, end) && isPawnPromotion(start, end) };
    const std::vector<char> promotable {{'r','b','n','q'}};
    valid = valid && std::any_of(promotable.begin(), promotable.end(), 
        [pieceType](char t) {
            return t == pieceType;
        });
    if(valid) {
        movePiece(start, end);
        piece(end) = ChessPiece(pieceType, otherPlayer());
    }
    return valid;
}

std::vector<ChessPosition> Chess::everyLegalMoveFrom(ChessPosition start) {
    std::vector<ChessPosition> moves;
    if(!piece(start).isNull() && playerTurn() == piece(start).getPlayer()) {
        moves = everyOpenMoveFrom(start);
        moves.erase(std::remove_if(moves.begin(), moves.end(), [this, start](ChessPosition end) {
            return this->hypotheticalCheck(start, end) || this->castleThreatened(start, end);
        }), moves.end());
    }
    return moves;
}

bool Chess::castleThreatened(ChessPosition start, ChessPosition end) {
    bool threatened { false };
    if(isCastleAttempt(start, end)) {
        ChessPosition middle((end.x - start.x) / 2, start.y);
        threatened = hypotheticalCheck(start, middle);
    }
    return threatened;
}

std::vector<Chess> Chess::everyHypotheticalGame() {
    std::vector<Chess> games;
    for(int x = 0; x < boardSize; x++) {
        for(int y = 0; y < boardSize; y++) {
            ChessPosition start(x, y);
            for(auto end : everyLegalMoveFrom(start)) {
                if(isPawnPromotion(start, end)) {
                    const std::vector<char> promoteable = {'r','b','n','q'};
                    for(char type : promoteable) {
                        games.push_back({start, end, type, board, turnCount});
                    }
                } else {
                    games.push_back({start, end, board, turnCount});
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
        case 'p':
            positions = pawnMoves(start);
            break;
        case 'r':
            positions = rookMoves(start);
            break;
        case 'b':
            positions = bishopMoves(start);
            break;
        case 'q':
            positions = queenMoves(start);
            break;
        case 'n':
            positions = knightMoves(start);
            break;
        case 'k':
            positions = kingMoves(start);
            break;
    }
    return positions;
}

void Chess::pawnCaptures(ChessPosition start, std::vector<ChessPosition>& positions) {
    int yDirection { pawnDirection(piece(start).getPlayer()) };
    const std::vector<ChessPosition> captureChecks = {ChessPosition(1, yDirection), ChessPosition(-1, yDirection)};
    for(ChessPosition offset : captureChecks) {
        ChessPosition position{start + offset};
        if(position.onBoard() && !piece(position).isNull() && piece(position).getPlayer() != piece(start).getPlayer()) {
            positions.push_back(position);
        }
    }
}

bool Chess::enPassant(ChessPosition start, ChessPosition end) {
    int yDirection = pawnDirection(piece(start).getPlayer());
    bool valid = piece(start).getType() == 'p';
    valid = valid && piece(end).enPassant() && piece(end).getPlayer() != piece(start).getPlayer();
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
        standardMove.onBoard() && piece(standardMove).isNull() 
    };

    if(standardClear) {
        positions.push_back(standardMove);
    }

    const bool doubleClear { 
        standardClear && doubleMove.onBoard() && 
        piece(doubleMove).isNull() && !piece(start).hasMoved() 
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
            reachedEnd = !position.onBoard() || !this->piece(position).isNull(); 
            if(position.onBoard() && (piece(position).isNull() || piece(position).getPlayer() != piece(start).getPlayer())) {
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
        if(position.onBoard() && (piece(position).isNull() || piece(position).getPlayer() != piece(start).getPlayer())) {
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
    valid = valid && piece(start).getType() == 'k' && !piece(start).hasMoved();
    valid = valid && std::abs(start.x - end.x) == 2;
    if(valid) {
        ChessPosition rookPosition(0, 0);
        ChessPosition kingDirection(0, 0);
        if(end.x > start.x) {
            rookPosition = ChessPosition(boardSize - 1, start.y);
            kingDirection = ChessPosition(1, 0);
        } else if(end.x < start.x) {
            rookPosition = ChessPosition(0, start.y);
            kingDirection = ChessPosition(-1, 0);
        }

        ChessPosition rookDirection = kingDirection * -1;
        valid = valid && !piece(rookPosition).isNull() && piece(rookPosition).getType() == 'r' && !piece(rookPosition).hasMoved();
        for(ChessPosition pos = start + kingDirection; pos != end && valid; pos += kingDirection) {
            valid = valid && piece(pos).isNull();
        }
        for(ChessPosition pos = rookPosition + rookDirection; pos != end + rookDirection && valid; pos += rookDirection) {
            valid = valid && piece(pos).isNull();
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

bool Chess::inCheck(int player) {
    bool check { false };
    for(int y = 0 ; y < boardSize && !check; y++) {
        for(int x = 0; x < boardSize && !check; x++) {
            if(!piece({x, y}).isNull() && piece({x, y}).getPlayer() != player) {
                std::vector<ChessPosition> threatened = everyOpenMoveFrom({x, y});
                check = check || std::any_of(threatened.begin(), threatened.end(), [this, player](ChessPosition position) {
                    return this->piece(position).getPlayer() == player && 
                            this->piece(position).getType() == 'k';
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
    const bool check = inCheck(piece(end).getPlayer());
    board = cachedBoard;
    return check;
}

bool Chess::playerStuck() {
    bool stuck { true };
    for(int y = 0; y < boardSize && stuck; y++) {
        for(int x = 0; x < boardSize && stuck; x++) {
            stuck = stuck && everyLegalMoveFrom({x, y}).empty();
        }
    }
    return stuck;
}


EndState Chess::endState() {
    EndState state;
    const bool stuck = playerStuck();
    const bool check = inCheck(playerTurn());
    if(stuck && check) {
        state = EndState("checkmate", otherPlayer());
    } else if(stuck && !check) {
        state = EndState("stalemate");
    }
    return state;
}

