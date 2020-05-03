#ifndef CHESS_HPP
#define CHESS_HPP

#include <string>
#include <array>
#include <vector>

#include "ChessPosition.hpp"
#include "ChessPiece.hpp"

const std::array<std::array<ChessPiece, boardSize>, boardSize> defaultBoard {
    {
        {ChessPiece('r', 1), ChessPiece('n', 1), ChessPiece('b', 1), ChessPiece('q', 1), ChessPiece('k', 1), ChessPiece('b', 1), ChessPiece('n', 1), ChessPiece('r', 1)},
        {ChessPiece('p', 1), ChessPiece('p', 1), ChessPiece('p', 1), ChessPiece('p', 1), ChessPiece('p', 1), ChessPiece('p', 1), ChessPiece('p', 1), ChessPiece('p', 1)},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece()},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece()},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece()},
        {ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece(), ChessPiece()},
        {ChessPiece('p', 0), ChessPiece('p', 0), ChessPiece('p', 0), ChessPiece('p', 0), ChessPiece('p', 0), ChessPiece('p', 0), ChessPiece('p', 0), ChessPiece('p', 0)},
        {ChessPiece('r', 0), ChessPiece('n', 0), ChessPiece('b', 0), ChessPiece('q', 0), ChessPiece('k', 0), ChessPiece('b', 0), ChessPiece('n', 0), ChessPiece('r', 0)}
    }
};

class Chess {
    public:
        Chess();
        int getTurnCount();
        ChessPiece getPiece(std::string position);
        ChessPiece getPiece(ChessPosition position);
        bool move(std::string start, std::string end);
        bool move(ChessPosition start, ChessPosition end);

    private:
        ChessPiece& piece(ChessPosition position);
        std::array<std::array<ChessPiece, boardSize>, boardSize> board;
        bool moveValid(ChessPosition start, ChessPosition end);
        std::vector<ChessPosition> everyOpenMoveFrom(ChessPosition start);
};

#endif