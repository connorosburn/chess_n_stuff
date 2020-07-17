#include <emscripten/bind.h>
#include "../GameEngines/Chess/Chess.hpp"
#include "../AI/AI.hpp"
#include "../GameEngines/Game.hpp"
#include <memory>

std::shared_ptr<Game> game;

bool move(std::string gameMove) {
    return game->move(gameMove);
}

std::string endState() {
    return game->endState();
}

void aiMove() {
    game = AI(game);
}

std::string playerTurn() {
    return Game::playerString.at(game->playerTurn());
}

void newGame(std::string gameType) {
    if(gameType == "chess") {
        game = std::shared_ptr<Game>(new Chess());
    }
}

void fromSnapshot(std::string gameType, std::string snapshot) {
    if(gameType == "chess") {
        game = std::shared_ptr<Game>(new Chess(snapshot));
    }
}

std::string getSnapshot() {
    return game->getSnapshot();
}

std::string everyLegalMove() {
    return game->everyLegalMove();
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("move", &move);
    emscripten::function("endState", &endState);
    emscripten::function("aiMove", &aiMove);
    emscripten::function("playerTurn", &playerTurn);
    emscripten::function("newGame", &newGame);
    emscripten::function("fromSnapshot", &fromSnapshot);
    emscripten::function("getSnapshot", &getSnapshot);
    emscripten::function("everyLegalMove", &everyLegalMove);
}