#ifndef CONNECT_FOUR_HPP
#define CONNECT_FOUR_HPP

enum class Player{One, Two, Null};

const int boardWidth = 7;
const int boardHeight = 6;

const std::vector<std::vector<Player>> defaultBoard(boardHeight, std::vector<Player>(boardWidth, Player::Null));

class ConnectFour {
    public:
        ConnectFour();
        bool move(int column);

    private:
    std::vector<std::vector<Player> board;
}

#endif