#include "Chess.hpp"

Chess::Chess(): board(defaultBoard) {

}

int Chess::getTurnCount() {
    return 0;
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
    }
    return valid;
}

bool Chess::moveValid(ChessPosition start, ChessPosition end) {
    std::vector<ChessPosition> moves = everyOpenMoveFrom(start);
    return std::any_of(moves.begin(), moves.end(),
        [end](ChessPosition position) {
            return position == end;
        }
    );
}

std::vector<ChessPosition> Chess::everyOpenMoveFrom(ChessPosition start) {
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