#include "Chess.hpp"

Chess::Chess(): board(defaultBoard), turnCount(0) {

}

int Chess::getTurnCount() {
    return turnCount;
}

int Chess::playerTurn() {
    return turnCount % 2;
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

bool Chess::move(std::string start, std::string end) {
    return move(ChessPosition(start), ChessPosition(end));
}

bool Chess::move(ChessPosition start, ChessPosition end) {
    bool valid = moveValid(start, end);
    if(valid) {
        piece(end) = piece(start);
        piece(start) = ChessPiece();
        piece(end).move();
        turnCount++;
    }
    return valid;
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
    }
    return positions;
}

std::vector<ChessPosition> Chess::pawnMoves(ChessPosition start) {
    int yDirection;
    if(piece(start).getPlayer() == 0) {
        yDirection = -1;
    } else if(piece(start).getPlayer() == 1) {
        yDirection = 1;
    }

    std::vector<ChessPosition> positions;

    ChessPosition standardMove{start + ChessPosition(0, yDirection)};
    ChessPosition doubleMove{start + (ChessPosition(0, yDirection) * 2)};

    bool standardClear = standardMove.onBoard() && piece(standardMove).isNull();
    if(standardClear) {
        positions.push_back(standardMove);
    }

    bool doubleClear = standardClear && doubleMove.onBoard() && piece(doubleMove).isNull() && !piece(start).hasMoved();
    if(doubleClear) {
        positions.push_back(doubleMove);
    }

    std::vector<ChessPosition> captureChecks = {ChessPosition(1, yDirection), ChessPosition(-1, yDirection)};
    for(ChessPosition offset : captureChecks) {
        ChessPosition position{start + offset};
        if(position.onBoard() && !piece(position).isNull() && piece(position).getPlayer() != piece(start).getPlayer()) {
            positions.push_back(position);
        }
    }

    return positions;
}

std::vector<ChessPosition> Chess::searchAlongVectors(ChessPosition start, std::vector<ChessPosition> searchVectors) {
    std::vector<ChessPosition> positions;
    for(auto vector : searchVectors) {
        auto positionOpen = [this](ChessPosition position) -> bool {
            return position.onBoard() && this->piece(position).isNull();
        };
        for(ChessPosition position = start + vector; positionOpen(position); position += vector) {
            positions.push_back(position);
        }
    }
    return positions;
}

std::vector<ChessPosition> Chess::rookMoves(ChessPosition start) {
    std::vector<ChessPosition> searchVectors {
        {ChessPosition(1, 0), ChessPosition(-1, 0), ChessPosition(0, 1), ChessPosition(0, -1)}
    };
    return searchAlongVectors(start, searchVectors);
}

std::vector<ChessPosition> Chess::bishopMoves(ChessPosition start) {
    std::vector<ChessPosition> searchVectors {
        {ChessPosition(1, 1), ChessPosition(-1, 1), ChessPosition(1, -1), ChessPosition(-1, -1)}
    };
    return searchAlongVectors(start, searchVectors);
}

std::vector<ChessPosition> Chess::queenMoves(ChessPosition start) {
    std::vector<ChessPosition> searchVectors {
        {ChessPosition(1, 0), ChessPosition(-1, 0), ChessPosition(0, 1), ChessPosition(0, -1),
        ChessPosition(1, 1), ChessPosition(-1, 1), ChessPosition(1, -1), ChessPosition(-1, -1)}
    };
    return searchAlongVectors(start, searchVectors);
}