#ifndef CHECKERS_PIECE_HPP
#define CHECKERS_PIECE_HPP

#include "../Game.hpp"
#include "../../json.hpp"

enum class CheckersType {Soldier, King, Null};

class CheckersPiece {
    public:
        CheckersPiece();
        CheckersPiece(Player playerColor);
        CheckersPiece(nlohmann::json snapshot);
        void makeKing();
        nlohmann::json getSnapshot() const;
        CheckersType getType() const;
        Player getPlayer() const;

    private:
        CheckersType type;
        Player player;
        static std::map<CheckersType, std::string> checkersTypeString;
        CheckersType checkersTypeFromString(std::string typeString);
};

#endif