#include <emscripten/bind.h>
#include "../GameEngines/Chess/Chess.hpp"
#include "../AI/AI.hpp"
#include "../GameEngines/Game.hpp"
#include <memory>

Chess chess;

struct DisplayPiece {
    DisplayPiece() {};
    DisplayPiece(ChessPiece piece, std::vector<ChessPosition> moves):
    type({piece.getType()}), null(piece.isNull()), legalMoves(moves) {
        if(piece.getPlayer() == 0) {
            player = "white";
        } else if(piece.getPlayer() == 1) {
            player = "black";
        }
        if(null) {
            type = " ";
        }
    };
    std::string type;
    std::string player;
    bool null;
    std::vector<ChessPosition> legalMoves;
};

std::vector<DisplayPiece> getBoard() {
    std::vector<DisplayPiece> pieces;
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

bool promotePawn(ChessPosition start, ChessPosition end, std::string pieceType) {
    return chess.move(start, end, pieceType[0]);
}

bool isPawnPromotion(ChessPosition start, ChessPosition end) {
    return chess.isPawnPromotion(start, end);
}

EndState endState() {
    return chess.endState();
}

void aiMove() {
    std::shared_ptr<Chess> newGame = std::dynamic_pointer_cast<Chess>(AI(&chess));
    chess = *newGame;
}

std::string playerTurn() {
    if(chess.playerTurn() == 0) {
        return "white";
    } else {
        return "black";
    }
}

void resetGame() {
    chess = Chess();
}

void fromSnapshot(std::string snapshot) {
    chess = Chess(snapshot);
}

std::string serialize() {
    return chess.serialize();
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::class_<ChessPosition>("Position")
        .constructor<int, int>()
        .property("x", &ChessPosition::x)
        .property("y", &ChessPosition::y)
        ;
    emscripten::register_vector<ChessPosition>("PositionVector");

    emscripten::value_object<DisplayPiece>("Piece")
        .field("type", &DisplayPiece::type)
        .field("player", &DisplayPiece::player)
        .field("null", &DisplayPiece::null)
        .field("legalMoves", &DisplayPiece::legalMoves)
        ;
    emscripten::register_vector<DisplayPiece>("PieceVector");

    emscripten::value_object<EndState>("EndState")
        .field("winner", &EndState::winner)
        .field("condition", &EndState::condition)
        ;

    emscripten::function("getBoard", &getBoard);
    emscripten::function("move", &move);
    emscripten::function("promotePawn", &promotePawn);
    emscripten::function("endState", &endState);
    emscripten::function("isPawnPromotion", &isPawnPromotion);
    emscripten::function("aiMove", &aiMove);
    emscripten::function("playerTurn", &playerTurn);
    emscripten::function("resetGame", &resetGame);
    emscripten::function("fromSnapshot", &fromSnapshot);
    emscripten::function("serialize", &serialize);
}