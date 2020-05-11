#ifndef NODE_HPP
#define NODE_HPP

#include "../GameEngine/Chess.hpp"
#include <map>
#include <memory>

struct ChessMove {
    ChessMove(ChessPosition startPos, ChessPosition endPos):
    start(startPos), end(endPos) {};
    ChessMove(ChessPosition startPos, ChessPosition endPos, char promotionType):
    ChessMove(startPos, endPos) {promotion = promotionType;};
    bool operator ==(ChessMove move) const {
        return move.start == start && move.end == end && move.promotion == promotion;
    }
    bool operator >(ChessMove move) const {
        return true;
    }
    bool operator <(ChessMove move) const {
        return true;
    }
    ChessPosition start;
    ChessPosition end;
    char promotion;
};

class Node {
    public:
        Node(Chess chessGame);
        std::map<ChessMove, std::shared_ptr<Node>> getChildren();
        std::vector<ChessMove> legalMoves();
        double localScore(int player);
        double depthScore(int player);

    private:
        double scorePieces(int player);
        std::map<ChessMove, std::shared_ptr<Node>> children;
        Chess chess;
};

#endif