#include "AI.hpp"
#include <random>

double minimize(Game* game, double max, double min, int searchDepth);

double maximize(Game* game, double max, double min, int searchDepth) {
    if(searchDepth == 0 || game->over()) {
        max = game->getScore(game->playerTurn());
    } else {
        for(auto child : game->everyHypotheticalGame()) {
            double score = minimize(child, max, min, searchDepth - 1);
            delete child;
            if(score >= min) {
                return min;
            } else if(score > max) {
                max = score;
            }
        }
    }
    return max;
}

double minimize(Game* game, double max, double min, int searchDepth) {
    if(searchDepth == 0 || game->over()) {
        min = game->getScore(game->otherPlayer());
    } else {
        for(auto child : game->everyHypotheticalGame()) {
            double score = maximize(child, max, min, searchDepth - 1);
            delete child;
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
    double score { 0 };
    auto games = rootGame->everyHypotheticalGame();
    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(games.begin(), games.end(), rng);
    Game* bestGame { nullptr };
    for(auto game : games) {
        double newScore = minimize(game, score, 1.1, searchDepth - 1);
        Game* oldGame = game;
        if(newScore > score || bestGame == nullptr) {
            score = newScore;
            oldGame = bestGame;
            bestGame = game;
        }
        if(oldGame != nullptr) {
            delete oldGame;
        }
    }
    return std::shared_ptr<Game>(bestGame);
}