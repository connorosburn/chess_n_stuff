#include "AI.hpp"
#include <memory>
#include <iostream>

ChessMove AI(Chess chess, int searchDepth) {
    std::shared_ptr<Node> root(new Node(chess));
    std::vector<std::shared_ptr<Node>> deepestNodes = {root};
    for(int i = 0; i < searchDepth; i++) {
        std::vector<std::shared_ptr<Node>> newDeepest;
        for(auto& node : deepestNodes) {
            std::map<ChessMove, std::shared_ptr<Node>> newNodes { node->getChildren() };
            for(auto& [move, newNode] : newNodes) {
                newDeepest.push_back(newNode);
            }
        }
        deepestNodes = newDeepest;
    }

    double highestScore = 0;
    ChessMove bestMove(ChessPosition(0, 0), ChessPosition(0, 0));
    for(auto& [move, node] : root->getChildren()) {
        double score = node->depthScore(chess.playerTurn());
        if(score > highestScore) {
            highestScore = score;
            bestMove = move;
        }
    }
    std::cout<<"AI Move:\n"<<bestMove.start.x<<" "<<bestMove.start.y<<std::endl<<bestMove.end.x<<" "<<bestMove.end.y<<std::endl;
    return bestMove;
}