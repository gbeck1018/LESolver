#pragma once

#include "main.hpp"

#define WORDLE_NUM_SPACES   5
#define NERDLE_NUM_SPACES   8
#define NUM_OUTCOMES        3

static unordered_map<GameType, int> game_guess_len_map = 
{
    {GameType::Wordle, WORDLE_NUM_SPACES},
    {GameType::Nerdle, NERDLE_NUM_SPACES},
};

int hint_string_to_idx(int guess_len, string const& hint);
string get_best_guess(int guess_len, 
    unordered_map<string, double>& guesses_map);
void cull_solution_list(int guess_len, unordered_map<string, 
    double>& guesses_map, string const& guess, int hint);
