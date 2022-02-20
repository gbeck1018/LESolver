#pragma once

#include "main.hpp"

void get_possible_guesses_map(GameType type, 
    unordered_map<string, double>& guesses_map);
string get_hardcoded_first_guess(GameType type);
