#include "game.h"

#pragma once

std::vector<std::string> SplitCommand(const std::string& cmd);

void Parse(std::vector<std::string>& cmd_vector, Game& new_game);