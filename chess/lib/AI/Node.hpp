#ifndef NODE_HPP
#define NODE_HPP

#include "../GameEngine/Chess.hpp"
#include <map>
#include <memory>

class Node {
    public:
        Node(Chess chessGame);
        std::vector<std::shared_ptr<Node>> getChildren();
        double localScore(int player);
        double minimize(double max, double min, int searchDepth);
        double maximize(double max, double min, int searchDepth);

    private:
        double scorePieces(int player);
        std::vector<std::shared_ptr<Node>> children;
        Chess chess;
};

#endif