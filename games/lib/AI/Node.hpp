#ifndef NODE_HPP
#define NODE_HPP

#include "../GameEngines/Chess/Chess.hpp"
#include <map>
#include <memory>

class Node {
    public:
        Node(std::shared_ptr<Game> newGame);
        std::vector<std::shared_ptr<Node>> getChildren();
        double minimize(double max, double min, int searchDepth);
        double maximize(double max, double min, int searchDepth);

    private:
        double scorePieces(int player);
        std::vector<std::shared_ptr<Node>> children;
        std::shared_ptr<Game> game;
};

#endif