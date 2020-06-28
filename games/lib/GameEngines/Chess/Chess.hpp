#ifndef CHESS_HPP
#define CHESS_HPP

#include <string>
#include <array>
#include <vector>

#include "ChessPosition.hpp"
#include "ChessPiece.hpp"
#include "../Game.hpp"

const std::vector<std::vector<ChessPiece>> defaultBoard {
    {
        {
            ChessPiece(PieceType::Rook, Player::Black), 
            ChessPiece(PieceType::Knight, Player::Black), 
            ChessPiece(PieceType::Bishop, Player::Black), 
            ChessPiece(PieceType::Queen, Player::Black), 
            ChessPiece(PieceType::King, Player::Black), 
            ChessPiece(PieceType::Bishop, Player::Black), 
            ChessPiece(PieceType::Knight, Player::Black), 
            ChessPiece(PieceType::Rook, Player::Black)
        },
        std::vector<ChessPiece>(boardSize, ChessPiece(PieceType::Pawn, Player::Black)),
        std::vector<ChessPiece>(boardSize, ChessPiece()),
        std::vector<ChessPiece>(boardSize, ChessPiece()),
        std::vector<ChessPiece>(boardSize, ChessPiece()),
        std::vector<ChessPiece>(boardSize, ChessPiece()),
        std::vector<ChessPiece>(boardSize, ChessPiece(PieceType::Pawn, Player::White)),
        {
            ChessPiece(PieceType::Rook, Player::White), 
            ChessPiece(PieceType::Knight, Player::White), 
            ChessPiece(PieceType::Bishop, Player::White), 
            ChessPiece(PieceType::Queen, Player::White), 
            ChessPiece(PieceType::King, Player::White), 
            ChessPiece(PieceType::Bishop, Player::White), 
            ChessPiece(PieceType::Knight, Player::White), 
            ChessPiece(PieceType::Rook, Player::White)
        }
    }
};

class Chess : public Game {
    public:
        Chess();
        Chess(std::vector<std::vector<ChessPiece>> chessBoard, int turnNumber);
        Chess(std::string snapshot);
        std::string getSnapshot();
        int getTurnCount();
        bool inCheck(Player player);
        std::vector<std::vector<ChessPiece>> getBoard();
        std::vector<ChessPosition> everyLegalMoveFrom(ChessPosition start);
        std::vector<std::shared_ptr<Game>> everyHypotheticalGame();
        ChessPiece getPiece(std::string position);
        ChessPiece getPiece(ChessPosition position);
        ChessPiece getPiece(int x, int y);
        bool move(std::string start, std::string end);
        bool move(std::string start, std::string end, PieceType pieceType);
        bool move(ChessPosition start, ChessPosition end);
        bool move(ChessPosition start, ChessPosition end, PieceType pieceType);
        bool move(std::string chessMove);
        bool isPawnPromotion(ChessPosition start, ChessPosition end);
        EndState endState();
        double getScore(Player player);

    private:
        Chess(ChessPosition start, ChessPosition end, Chess chessGame);
        Chess(ChessPosition start, ChessPosition end, PieceType type, Chess chessGame);
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

        double scorePieces(Player player);
};

#endif