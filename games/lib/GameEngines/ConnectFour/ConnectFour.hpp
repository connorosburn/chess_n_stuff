#ifndef CONNECT_FOUR_HPP
#define CONNECT_FOUR_HPP
#include "../Game.hpp"
#include "../../json.hpp"
#include <vector>

const int connectFourHeight { 6 };
const int connectFourWidth { 7 };

const std::vector<std::vector<Player>> defaultConnectFourBoard(
    connectFourHeight,
    std::vector<Player>(connectFourWidth, Player::Null)
);

class ConnectFour : public Game {
    public:
        ConnectFour();
        ConnectFour(std::string snapshot);
        std::string getSnapshot();
        std::string endState();
        Player checkWinner();
        double getScore(Player player);
        std::vector<Game*> everyHypotheticalGame();
        int getTurnCount();
        bool move(int column);
        bool move(std::string gameMove);
        std::string everyLegalMove();
        bool over();

    private:
        ConnectFour(ConnectFour proto, int column);
        std::vector<std::vector<Player>> board;
        void dropPiece(int column);
        bool boardFull();
};

#endif