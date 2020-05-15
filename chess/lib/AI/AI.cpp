#include "AI.hpp"
#include <memory>
#include <iostream>
#include <future>

ChessMove AI(Chess chess, int searchDepth) {
    std::shared_ptr<Node> root(new Node(chess));

    double score {0};
    ChessMove move(ChessPosition(0, 0), ChessPosition(0, 0));
    for(auto[moveChoice, child] : root->getChildren()) {
        double newScore = child->minimize(score, 1.1, searchDepth);
        if(newScore > score) {
            score = newScore;
            move = moveChoice;
        }
    }
    std::cout<<"AI Move:\n"<<move.start.x<<" "<<move.start.y<<std::endl<<move.end.x<<" "<<move.end.y<<std::endl;
    return move;
}