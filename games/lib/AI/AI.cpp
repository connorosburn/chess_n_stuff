#include "AI.hpp"
#include <random>

double minimize(std::shared_ptr<Game> game, double max, double min, int searchDepth);

double maximize(std::shared_ptr<Game> game, double max, double min, int searchDepth) {
    if(searchDepth == 0) {
        max = game->getScore(game->playerTurn());
    } else {
        for(auto child : game->everyHypotheticalGame()) {
            double score = minimize(child, max, min, searchDepth - 1);
            if(score >= min) {
                return min;
            } else if(score > max) {
                max = score;
            }
        }
    }
    return max;
}

double minimize(std::shared_ptr<Game> game, double max, double min, int searchDepth) {
    if(searchDepth == 0) {
        min = game->getScore(game->otherPlayer());
    } else {
        for(auto child : game->everyHypotheticalGame()) {
            double score = maximize(child, max, min, searchDepth - 1);
            if(score <= max) {
                return max;
            } else if(score < min) {
                min = score;
            }
        }
    }
    return min;
}

std::shared_ptr<Game> AI(std::shared_ptr<Game> rootGame, int searchDepth) {
    double score {0};
    auto games = rootGame->everyHypotheticalGame();
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(games.begin(), games.end(), rng);
    std::shared_ptr<Game> bestGame;
    for(auto game : games) {
        double newScore = minimize(game, score, 1.1, searchDepth - 1);
        if(newScore > score) {
            score = newScore;
            bestGame = game;
        }
    }
    return bestGame;
}