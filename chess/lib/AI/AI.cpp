#include "AI.hpp"
#include <memory>
#include <iostream>

Chess AI(Chess chess, int searchDepth) {
    double score {0};
    Chess bestGame {chess};
    for(auto& game : chess.everyHypotheticalGame()) {
        double newScore = Node(game).minimize(score, 1.1, searchDepth);
        if(newScore > score) {
            score = newScore;
            bestGame = game;
        }
    }
    return bestGame;
}