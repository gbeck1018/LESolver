#include "wordlist.hpp"

#include <fstream>
using std::ifstream;

/**
 * Read in the guess set
 *
 * @param[out] guesses_map This function initializes the keys to INT_MAX
 * @return void
 */
void read_possible_guesses_map(string file_name,
    unordered_map<string, double>& guesses_map)
{
    // Open our solutions document
    ifstream possible_words_file;
    possible_words_file.open(file_name);

    // Collect our solutions into a map
    string in_line;
    while (getline(possible_words_file, in_line))
    {
        guesses_map[in_line] = UNINITIALIZED_ENTROPY;
    }
    possible_words_file.close();
}

/**
 * Read in a possible guess map
 *
 * @param[in] type The version of the game to use
 * @param[out] guesses_map The guess map to read in
 * @return void
 */
void get_possible_guesses_map(GameType type, 
    unordered_map<string, double>& guesses_map)
{
    switch (type)
    {
    case GameType::Wordle:
        read_possible_guesses_map("lib/wordle_wordlist.txt", guesses_map);
        return;
    case GameType::Nerdle:
        read_possible_guesses_map("lib/nerdle_wordlist.txt", guesses_map);
        return;
    default:
        throw "Unsupported game type";
        return;
    }
}

/**
 * Get a hardcoded first guess to save on compute time
 *
 * @param[in] type The version of the game to use
 * @return The guess
 */
string get_hardcoded_first_guess(GameType type)
{
    switch (type)
    {
    case GameType::Wordle:
        return "raise";
    case GameType::Nerdle:
        return "48-32=16";
    default:
        throw "Unsupported game type";
        return string();
    }
}
