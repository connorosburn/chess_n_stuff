#include "AI.hpp"
#include <random>
#include <iostream>

std::shared_ptr<Game> AI(Game* rootGame, int searchDepth) {
    double score {0};
    auto games = rootGame->everyHypotheticalGame();
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(games.begin(), games.end(), rng);
    std::shared_ptr<Game> bestGame;
    for(auto game : games) {
        double newScore = Node(game).minimize(score, 1.1, searchDepth - 1);
        if(newScore > score) {
            score = newScore;
            bestGame = game;
        }
    }
    return bestGame;
}