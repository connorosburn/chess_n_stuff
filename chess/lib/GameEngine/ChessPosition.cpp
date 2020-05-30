#include "ChessPosition.hpp"
#include <array>
#include <algorithm>

const std::array<char, 8> letteredPositions {{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}};

ChessPosition::ChessPosition(std::string notationString) {
    if(notationStringValid(notationString)) {
        valuesFromString(notationString);
    } else {
        throw "Invalid position string";
    }
}

ChessPosition::ChessPosition(int xValue, int yValue): x(xValue), y(yValue) {

}

bool ChessPosition::notationStringValid(std::string notationString) {
    bool valid = notationString.size() == 2;
    valid = valid && std::any_of(letteredPositions.begin(), letteredPositions.end(), 
        [notationString](char c) { 
            return notationString.front() == c; 
        } 
    );
	try {
        std::stoi(notationString.substr(1, 1));
	}
	catch(const std::exception &e) {
		valid = false;
	}
    return valid;
}

void ChessPosition::valuesFromString(std::string notationString) {
    auto position = std::find(letteredPositions.begin(), letteredPositions.end(), notationString.front());
    x = std::distance(letteredPositions.begin(), position);
    y = boardSize - std::stoi(notationString.substr(1, 1));
}

bool ChessPosition::onBoard() const {
    return x >= 0 && x < boardSize && y >= 0 && y < boardSize;
}

bool ChessPosition::operator==(ChessPosition position) const {
    return x == position.x && y == position.y;
}

bool ChessPosition::operator!=(ChessPosition position) const {
    return !(*this == position);
}

ChessPosition ChessPosition::operator+(ChessPosition position) const {
    return ChessPosition(x + position.x, y + position.y);
}

void ChessPosition::operator+=(ChessPosition position) {
    *this = *this + position;
}

ChessPosition ChessPosition::operator*(int multiplier) const {
    return ChessPosition(x * multiplier, y * multiplier);
}