#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

#include <string>

const int boardSize {8};

class ChessPosition {
    public:
        ChessPosition(std::string notationString);
        ChessPosition(int xValue, int yValue);
        int x;
        int y;

    private:
        bool notationStringValid(std::string notationString);
        void valuesFromString(std::string notationString);
};

#endif