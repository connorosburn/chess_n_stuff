#include <emscripten/bind.h>
#include "../GameEngine/Chess.hpp"


Chess chess;

struct ExposedPiece {
    ExposedPiece() {};
    ExposedPiece(ChessPiece piece, std::vector<ChessPosition> moves):
    type({piece.getType()}), player(piece.getPlayer()), null(piece.isNull()), legalMoves(moves) {};
    std::string type;
    int player;
    bool null;
    std::vector<ChessPosition> legalMoves;
};

std::vector<ExposedPiece> getBoard() {
    std::vector<ExposedPiece> pieces;
    auto board = chess.getBoard();
    for(int y = 0; y < board.size(); y++) {
        for(int x = 0; x < board[y].size(); x++) {
            pieces.emplace_back(board[y][x], chess.everyLegalMoveFrom({x, y}));
        }
    }
    return pieces;
}

bool move(ChessPosition start, ChessPosition end) {
    return chess.move(start, end);
}

EndState endState() {
    return chess.endState();
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::class_<ChessPosition>("Position")
        .constructor<int, int>()
        .property("x", &ChessPosition::x)
        .property("y", &ChessPosition::y)
        ;
    emscripten::register_vector<ChessPosition>("PositionVector");

    emscripten::value_object<ExposedPiece>("Piece")
        .field("type", &ExposedPiece::type)
        .field("player", &ExposedPiece::player)
        .field("null", &ExposedPiece::null)
        .field("legalMoves", &ExposedPiece::legalMoves)
        ;
    emscripten::register_vector<ExposedPiece>("PieceVector");

    emscripten::value_object<EndState>("EndState")
        .field("winner", &EndState::winner)
        .field("condition", &EndState::condition)
        ;

    emscripten::function("getBoard", &getBoard);
    emscripten::function("move", &move);
    emscripten::function("endState", &endState);
}