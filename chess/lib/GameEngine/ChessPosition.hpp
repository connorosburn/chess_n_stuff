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
        bool onBoard() const;
        bool operator==(const ChessPosition& position);
        ChessPosition operator+(const ChessPosition& position);
        void operator+=(const ChessPosition& position);
        ChessPosition operator*(int multiplier);

    private:
        bool notationStringValid(std::string notationString);
        void valuesFromString(std::string notationString);
};

#endif