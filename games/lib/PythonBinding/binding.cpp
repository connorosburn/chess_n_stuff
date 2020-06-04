#include <pybind11/pybind11.h>
#include "../GameEngine/Chess.hpp"


std::string move(std::string jsonBoard, int startX, int startY, int endX, int endY) {
    Chess chess(jsonBoard);
    std::string returnBoard;
    ChessPosition start(startX, startY);
    ChessPosition end(endX, endY);
    if(chess.move(start, end)) {
        returnBoard = chess.serialize();
    }
    return returnBoard;
}

std::string promotePawn(std::string jsonBoard, int startX, int startY, int endX, int endY, std::string pieceType) {
    Chess chess(jsonBoard);
    std::string returnBoard;
    ChessPosition start(startX, startY);
    ChessPosition end(endX, endY);
    if(chess.move(start, end, pieceType[0])) {
        returnBoard = chess.serialize();
    }
    return returnBoard;
}

std::string newGame() {
    return Chess().serialize();
}

PYBIND11_MODULE(cpp_chess, m) {
    m.def("new_game", &newGame);
    m.def("move", &move);
    m.def("promote_pawn", &promotePawn);
}