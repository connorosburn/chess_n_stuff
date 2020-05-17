#include "AI.hpp"
#include <random>

Chess AI(Chess chess, int searchDepth) {
    double score {0};
    Chess bestGame {chess};
    auto games = chess.everyHypotheticalGame();
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(games.begin(), games.end(), rng);
    for(auto game : games) {
        double newScore = Node(game).minimize(score, 1.1, searchDepth);
        if(newScore > score) {
            score = newScore;
            bestGame = game;
        }
    }
    return bestGame;
}