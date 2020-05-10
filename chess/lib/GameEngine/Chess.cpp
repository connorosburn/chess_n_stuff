#include "Chess.hpp"

Chess::Chess(std::vector<std::vector<ChessPiece>> chessBoard, int turnNumber):
board(chessBoard), turnCount(turnNumber) {

}

Chess::Chess(): Chess(defaultBoard, 0) {

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

bool Chess::move(ChessPosition start, ChessPosition end) {
    bool valid { moveValid(start, end) };
    if(valid) {
        movePiece(start, end);
    }
    return valid;
}

std::vector<ChessPosition> Chess::everyLegalMoveFrom(ChessPosition start) {
    std::vector<ChessPosition> moves { everyOpenMoveFrom(start) };
    moves.erase(std::remove_if(moves.begin(), moves.end(), [start, this](ChessPosition p) {
        return !this->moveValid(start, p);
    }), moves.end());
    return moves;
}

bool Chess::moveValid(ChessPosition start, ChessPosition end) {
    bool valid = piece(start).getPlayer() == playerTurn();
    if(valid) {
        std::vector<ChessPosition> moves = everyOpenMoveFrom(start);
        valid = std::any_of(moves.begin(), moves.end(),
            [end](ChessPosition position) {
                return position == end;
            }
        );
    }
    valid = valid && !hypotheticalCheck(start, end);
    return valid;
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
    valid = valid &&  end.onBoard();
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

std::vector<ChessPosition> Chess::kingMoves(ChessPosition start) {
    std::vector<ChessPosition> offsets {
        {
            ChessPosition(1, 0), ChessPosition(-1, 0), ChessPosition(0, 1), ChessPosition(0, -1),
            ChessPosition(1, 1), ChessPosition(-1, 1), ChessPosition(1, -1), ChessPosition(-1, -1)
        }
    };
    return checkIndividualOffsets(start, offsets);
}

bool Chess::inCheck(int player) {
    bool check { false };
    for(int i = 0 ; i < board.size() && !check; i++) {
        for(int j = 0; j < board[i].size() && !check; j++) {
            check = check || detectCheckFromPosition(j, i, player);
        }
    }
    return check;
}

bool Chess::detectCheckFromPosition(int x, int y, int player) {
    bool check { false };
    const bool enemyPiece { !board[y][x].isNull() && board[y][x].getPlayer() != player };
    if(enemyPiece) {
        std::vector<ChessPosition> moves { everyOpenMoveFrom({x, y}) };
        check = std::any_of(moves.begin(), moves.end(), [player, this](ChessPosition m) {
            return this->piece(m).getType() == 'k' && this->piece(m).getPlayer() == player;
        });
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


EndState Chess::endState() {
    const bool stuck = moveablePieces().empty();
    const bool check = inCheck(playerTurn());
    EndState state;
    if(stuck && check) {
        state = EndState("checkmate", otherPlayer());
    } else if(stuck && !check) {
        state = EndState("stalemate");
    }
    return state;
}

std::vector<ChessPosition> Chess::moveablePieces() {
    std::vector<ChessPosition> positions;
    for(int y = 0; y < board.size(); y++) {
        for(int x = 0; x < board[y].size(); x++) {
            std::vector<ChessPosition> openings { everyOpenMoveFrom({x, y}) };
            const bool anyLegal = std::any_of(openings.begin(), openings.end(), [this, x, y](ChessPosition p) {
                return this->moveValid({x, y}, p);
            });
            if(anyLegal) {
                positions.emplace_back(x, y);
            }
        }
    }
    return positions;
}