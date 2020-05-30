#ifndef CHESS_HPP
#define CHESS_HPP

#include <string>
#include <array>
#include <vector>

#include "ChessPosition.hpp"
#include "ChessPiece.hpp"

const std::vector<std::vector<ChessPiece>> defaultBoard {
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

struct EndState {
    EndState(std::string endCondition, int gameWinner):
        condition(endCondition), winner(gameWinner) {};
    EndState(std::string endCondition):
        EndState(endCondition, -1) {};
    EndState():
        EndState("", -1) {};
    std::string condition;
    int winner;
};

class Chess {
    public:
        Chess();
        Chess(std::vector<std::vector<ChessPiece>> chessBoard, int turnNumber);
        Chess(std::string jsonString);
        std::string serialize();
        int getTurnCount();
        int playerTurn();
        int otherPlayer();
        bool inCheck(int player);
        std::vector<std::vector<ChessPiece>> getBoard();
        std::vector<ChessPosition> everyLegalMoveFrom(ChessPosition start);
        std::vector<Chess> everyHypotheticalGame();
        ChessPiece getPiece(std::string position);
        ChessPiece getPiece(ChessPosition position);
        ChessPiece getPiece(int x, int y);
        bool move(std::string start, std::string end);
        bool move(std::string start, std::string end, char pieceType);
        bool move(ChessPosition start, ChessPosition end);
        bool move(ChessPosition start, ChessPosition end, char pieceType);
        bool isPawnPromotion(ChessPosition start, ChessPosition end);
        EndState endState();

    private:
        Chess(ChessPosition start, ChessPosition end, Chess chessGame);
        Chess(ChessPosition start, ChessPosition end, char type, Chess chessGame);
        int turnCount;
        ChessPiece& piece(ChessPosition position);
        std::vector<std::vector<ChessPiece>> board;
        void movePiece(ChessPosition start, ChessPosition end);
        void enPassantExceptions(ChessPosition start, ChessPosition end);
        bool moveValid(ChessPosition start, ChessPosition end);
        bool playerStuck();

        // will show all regular movement available from each position
        std::vector<ChessPosition> everyOpenMoveFrom(ChessPosition start);

        // helper methods for piece movement
        std::vector<ChessPosition> searchAlongVectors(ChessPosition start, std::vector<ChessPosition> searchVectors);
        std::vector<ChessPosition> checkIndividualOffsets(ChessPosition start, std::vector<ChessPosition> offsets);

        // helper methods for particular piece movement
        void pawnCaptures(ChessPosition start, std::vector<ChessPosition>& positions);
        void enPassantPositions(ChessPosition start, std::vector<ChessPosition>& positions);
        bool enPassant(ChessPosition start, ChessPosition end);
        bool isCastleAttempt(ChessPosition start, ChessPosition end);
        bool castleThreatened(ChessPosition start, ChessPosition end);


        // searches for regular movement for each piece
        std::vector<ChessPosition> pawnMoves(ChessPosition start);
        std::vector<ChessPosition> rookMoves(ChessPosition start);
        std::vector<ChessPosition> bishopMoves(ChessPosition start);
        std::vector<ChessPosition> queenMoves(ChessPosition start);
        std::vector<ChessPosition> knightMoves(ChessPosition start);
        std::vector<ChessPosition> kingMoves(ChessPosition start);

        // helper method for check detection
        bool hypotheticalCheck(ChessPosition start, ChessPosition end);
};

#endif