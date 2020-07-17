#ifndef AI_HPP
#define AI_HPP
#include <memory>
#include "../GameEngines/Game.hpp"

std::shared_ptr<Game> AI(std::shared_ptr<Game> rootGame, int searchDepth = 3);

#endif