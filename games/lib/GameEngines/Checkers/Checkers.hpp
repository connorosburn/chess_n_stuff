#ifndef CHECKERS_HPP
#define CHECKERS_HPP

#include "../Game.hpp"
#include "../../json.hpp"
#include "CheckersPiece.hpp"
#include <vector>

const int checkersBoardSize { 8 };

const std::vector<std::vector<CheckersPiece>> defaultCheckersBoard {{
    {
        CheckersPiece(), CheckersPiece(Player::White), CheckersPiece(), CheckersPiece(Player::White),
        CheckersPiece(), CheckersPiece(Player::White), CheckersPiece(), CheckersPiece(Player::White)
    },
    {
        CheckersPiece(Player::White), CheckersPiece(), CheckersPiece(Player::White), CheckersPiece(),
        CheckersPiece(Player::White), CheckersPiece(), CheckersPiece(Player::White), CheckersPiece()
    },
    {
        CheckersPiece(), CheckersPiece(Player::White), CheckersPiece(), CheckersPiece(Player::White),
        CheckersPiece(), CheckersPiece(Player::White), CheckersPiece(), CheckersPiece(Player::White)
    },
    std::vector<CheckersPiece>(checkersBoardSize, CheckersPiece()),
    std::vector<CheckersPiece>(checkersBoardSize, CheckersPiece()),
    {
        CheckersPiece(Player::Black), CheckersPiece(), CheckersPiece(Player::Black), CheckersPiece(),
        CheckersPiece(Player::Black), CheckersPiece(), CheckersPiece(Player::Black), CheckersPiece()
    },
    {
        CheckersPiece(), CheckersPiece(Player::White), CheckersPiece(), CheckersPiece(Player::White),
        CheckersPiece(), CheckersPiece(Player::White), CheckersPiece(), CheckersPiece(Player::White)
    },
    {
        CheckersPiece(Player::Black), CheckersPiece(), CheckersPiece(Player::Black), CheckersPiece(),
        CheckersPiece(Player::Black), CheckersPiece(), CheckersPiece(Player::Black), CheckersPiece()
    }
}};

class Checkers : public Game {
    public:
        Checkers();
        Checkers(std::string snapshot);
        std::vector<Game*> everyHypotheticalGame();
        int getTurnCount();
        std::string getSnapshot();
        bool move(std::string gameMove);
        std::string everyLegalMove();
        bool over();
        std::string endState();
        double getScore(Player player);



    private:
        Checkers(std::vector<std::vector<CheckersPiece>> newBoard, int turns);
        std::vector<std::vector<CheckersPiece>> board;
        int turnCount;
        std::vector<Game*> everyOutcomeFrom(int x, int y);
        std::vector<Game*> everyJumpOutcomeFrom(int x, int y, std::vector<std::vector<CheckersPiece>> runningBoard);
        nlohmann::json everySerializedMoveFrom(int x, int y);
};

#endif