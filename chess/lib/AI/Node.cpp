#include "Node.hpp"
#include <future>

Node::Node(Chess chessGame): chess(chessGame) {

}

std::vector<std::shared_ptr<Node>> Node::getChildren() {
    if(children.empty()) {
        for(Chess& game : chess.everyHypotheticalGame()) {
            children.emplace_back(new Node(game));
        }
    }
    return children;
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

double Node::maximize(double max, double min, int searchDepth) {
    if(searchDepth == 0) {
        max = localScore(chess.playerTurn());
    } else {
        for(auto child : getChildren()) {
            double score = child->minimize(max, min, searchDepth - 1);
            if(score >= min) {
                return min;
            } else if(score > max) {
                max = score;
            }
        }
    }
    return max;
}

double Node::minimize(double max, double min, int searchDepth) {
    if(searchDepth == 0) {
        min = localScore(chess.otherPlayer());
    } else {
        for(auto child : getChildren()) {
            double score = child->maximize(max, min, searchDepth - 1);
            if(score <= max) {
                return max;
            } else if(score < min) {
                min = score;
            }
        }
    }
    return min;
}