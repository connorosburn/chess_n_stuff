#ifndef GAME_HPP
#define GAME_HPP
#include <string>
#include <map>

enum class Player {White, Black, Null};

class Game {
    public:
        virtual double getScore(Player player) = 0;
        virtual std::vector<std::shared_ptr<Game>> everyHypotheticalGame() = 0;
        virtual int getTurnCount() = 0;
        Player playerTurn();
        Player otherPlayer();
        virtual std::string getSnapshot() = 0;
        virtual bool move(std::string gameMove) = 0;
        virtual std::string everyLegalMove() = 0;
        virtual std::string endState() = 0;
        static const std::map<Player, std::string> playerString;
        static Player playerFromString(std::string rawPlayer);
};

#endif