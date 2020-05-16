#include "AI.hpp"
#include <memory>

Chess AI(Chess chess, int searchDepth) {
    double score {0};
    Chess bestGame {chess};
    auto games = chess.everyHypotheticalGame();
    for(auto game : games) {
        Node node(game);
        double newScore = node.minimize(score, 1.1, searchDepth);
        if(newScore > score) {
            score = newScore;
            bestGame = game;
        }
    }
    return bestGame;
}