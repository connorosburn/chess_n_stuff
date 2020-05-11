#include "Node.hpp"

Node::Node(Chess chessGame): chess(chessGame) {

}

std::map<ChessMove, std::shared_ptr<Node>> Node::getChildren() {
    if(children.empty()) {
        for(ChessMove move : legalMoves()) {
            if(chess.isPawnPromotion(move.start, move.end)) {
                const std::vector<char> options = {'r', 'n', 'b', 'q'};
                for(char option : options) {
                    Chess chessCopy = chess;
                    chessCopy.move(move.start, move.end, option);
                    ChessMove promotionMove(move.start, move.end, option);
                    children.emplace(promotionMove, new Node(chessCopy));      
                }
            } else {
                Chess chessCopy = chess;
                chessCopy.move(move.start, move.end);
                children.emplace(move, new Node(chessCopy));
            }
        }
    }
    return children;
}

std::vector<ChessMove> Node::legalMoves() {
    std::vector<ChessMove> moves;
    for(ChessPosition& start : chess.moveablePieces()) {
        for(ChessPosition& end : chess.everyLegalMoveFrom(start)) {
            moves.emplace_back(start, end);
        }
    }
    return moves;
}

double Node::scorePieces(int player) {
    const std::map<char, int> values = {
        {'p', 1}, {'n', 3}, {'b', 3}, {'r', 5}, {'q', 9}
    };

    std::map<int, int> pieceTotals {
        {0, 0}, {1, 0}
    };

    for(auto row : chess.getBoard()) {
        for(auto piece : row) {
            if(!piece.isNull() && piece.getType() != 'k') {
                pieceTotals[piece.getPlayer()] += values.at(piece.getType());
            }
        }
    }

    int totalScore = pieceTotals[0] + pieceTotals[1];
    return static_cast<double>(pieceTotals[player]) / static_cast<double>(totalScore);
}

double Node::localScore(int player) {
    auto endState = chess.endState();
    double score;
    if(endState.condition == "") {
        score = scorePieces(player);
    } else if(endState.condition == "checkmate" && player == endState.winner) {
        score = 1;
    } else if(endState.condition == "checkmate" && player != endState.winner) {
        score = 0;
    } else {
        score = 0.5;
    }
    return score;
}

double Node::depthScore(int player) {
    double total {0};
    for(auto& [move, child] : children) {
        total += child->depthScore(player);
    }
    total += localScore(player);
    return total / static_cast<double>(children.size() + 1);
}