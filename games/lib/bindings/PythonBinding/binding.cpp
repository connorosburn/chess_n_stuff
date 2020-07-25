#include <pybind11/pybind11.h>
#include "../GameType.hpp"

std::string newGame(std::string gameType, std::string move) {
    std::string newSnapshot;
    std::shared_ptr<Game> game { GameType::gameTypes.at(gameType).newGame() };
    if(game->move(move)) {
        newSnapshot = game->getSnapshot();
    }
    return newSnapshot;
}

std::string fromSnapshot(std::string gameType, std::string move, std::string snapshot) {
    std::string newSnapshot;
    std::shared_ptr<Game> game { GameType::gameTypes.at(gameType).fromSnapshot(snapshot) };
    if(game->move(move)) {
        newSnapshot = game->getSnapshot();
    }
    return newSnapshot;
}

PYBIND11_MODULE(cpp_games, m) {
    m.def("new_game", &newGame);
    m.def("from_snapshot", &fromSnapshot);
}