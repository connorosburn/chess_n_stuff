#ifndef GAME_HPP
#define GAME_HPP
#include <string>

enum class Player {White, Black, Null};
std::string playerString(Player player);
Player playerFromString(std::string player);

struct EndState {
    EndState(std::string endCondition, Player gameWinner);
    EndState(std::string endCondition):
        EndState(endCondition, Player::Null) {};
    EndState():
        EndState("", Player::Null) {};
    std::string condition;
    Player winner;
};

class Game {
    public:
        virtual double getScore(Player player) = 0;
        virtual std::vector<std::shared_ptr<Game>> everyHypotheticalGame() = 0;
        virtual int getTurnCount() = 0;
        Player playerTurn();
        Player otherPlayer();
        virtual std::string getSnapshot() = 0;
        virtual bool move(std::string gameMove) = 0;
        virtual EndState endState() = 0;
};

#endif