#include "Node.hpp"

Node::Node(std::shared_ptr<Game> newGame): game(newGame) {

}

double Node::maximize(double max, double min, int searchDepth) {
    if(searchDepth == 0) {
        max = game->getScore(game->playerTurn());
    } else {
        for(auto child : game->everyHypotheticalGame()) {
            double score = Node(child).minimize(max, min, searchDepth - 1);
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
        min = game->getScore(game->otherPlayer());
    } else {
        for(auto child : game->everyHypotheticalGame()) {
            double score = Node(child).maximize(max, min, searchDepth - 1);
            if(score <= max) {
                return max;
            } else if(score < min) {
                min = score;
            }
        }
    }
    return min;
}