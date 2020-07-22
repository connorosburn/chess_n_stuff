#ifndef GAME_TYPE_HPP
#define GAME_TYPE_HPP
#include "../GameEngines/Game.hpp"
#include "../GameEngines/Chess/Chess.hpp"
#include "../GameEngines/TicTacToe/TicTacToe.hpp"

namespace GameType {
    struct GameType {
        GameType(
            std::function<std::shared_ptr<Game>()> newGameFunction, 
            std::function<std::shared_ptr<Game>(std::string)> fromSnapshotFunction,
            int aiSearchDepth
        ): newGame(newGameFunction), fromSnapshot(fromSnapshotFunction), searchDepth(aiSearchDepth) 
        {};

        GameType(): 
        newGame([](){return std::shared_ptr<Game>();}), fromSnapshot([](std::string n){return std::shared_ptr<Game>();}), searchDepth(0) 
        {};
        std::function<std::shared_ptr<Game>()> newGame;
        std::function<std::shared_ptr<Game>(std::string)> fromSnapshot;
        int searchDepth;
    };

    const GameType ChessGame(
        []() {
            return std::shared_ptr<Game>(new Chess());
        },
        [](std::string snapshot) {
            return std::shared_ptr<Game>(new Chess(snapshot));
        },
        3
    );

    const GameType TicTacToeGame(
        []() {
            return std::shared_ptr<Game>(new TicTacToe());
        },
        [](std::string snapshot) {
            return std::shared_ptr<Game>(new TicTacToe(snapshot));
        },
        10
    );

    const std::map<std::string, GameType> gameTypes {
        {"chess", ChessGame},
        {"tic-tac-toe", TicTacToeGame}
    };
};

#endif