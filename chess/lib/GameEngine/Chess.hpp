#ifndef CHESS_HPP
#define CHESS_HPP
#include <string>

class Chess {
    public:
        int getTurnCount();
        std::string getPiece(std::string position);
};

#endif