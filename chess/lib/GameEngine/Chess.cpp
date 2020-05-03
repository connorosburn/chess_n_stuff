#include "Chess.hpp"

int Chess::getTurnCount() {
    return 0;
}

std::string Chess::getPiece(std::string position) {
    return getPiece({position});
}