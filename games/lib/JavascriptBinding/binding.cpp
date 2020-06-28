#include <emscripten/bind.h>
#include "../GameEngines/Chess/Chess.hpp"
#include "../AI/AI.hpp"
#include "../GameEngines/Game.hpp"
#include <memory>

std::shared_ptr<Game> game;

bool move(std::string gameMove) {
    return game->move(gameMove);
}

EndState endState() {
    return game->endState();
}

void aiMove() {
    game = AI(game);
}

std::string playerTurn() {
    return playerString(game->playerTurn());
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

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::value_object<EndState>("EndState")
        .field("winner", &EndState::winner)
        .field("condition", &EndState::condition)
        ;
    emscripten::function("move", &move);
    emscripten::function("endState", &endState);
    emscripten::function("aiMove", &aiMove);
    emscripten::function("playerTurn", &playerTurn);
    emscripten::function("newGame", &newGame);
    emscripten::function("fromSnapshot", &fromSnapshot);
    emscripten::function("getSnapshot", &getSnapshot);
}