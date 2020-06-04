#ifndef GAME_HPP
#define GAME_HPP

class Game {
    public:
        virtual double getScore(int player) = 0;
        virtual std::vector<std::shared_ptr<Game>> everyHypotheticalGame() = 0;
        virtual int playerTurn() = 0;
        virtual int otherPlayer() = 0;
};

#endif