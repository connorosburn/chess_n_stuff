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
        double localScore(int player);
        double minimize(double max, double min, int searchDepth);
        double maximize(double max, double min, int searchDepth);

    private:
        double scorePieces(int player);
        std::vector<ChessMove> legalMoves();
        std::map<ChessMove, std::shared_ptr<Node>> children;
        Chess chess;
};

#endif