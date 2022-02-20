#include "wordlist.hpp"
#include "solve.hpp"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <getopt.h>

void usage(char* prog_name)
{
    cout << "Usage: " << prog_name << " [-v {w, n}]\n";
    cout << "\n";
    cout << "Solver for *le type games, such as Wordle and Nerdle\n";
    cout << "\n";
    cout << "Options:\n";
    cout << "\n";
    cout << "-v     - Define which version of the game you're playing.\n";
    cout << "           -v w: Wordle\n";
    cout << "           -v n: Nerdle\n";
    cout << "-h     - Show help.\n";
    cout << endl;
}

GameType get_game_type(char *c)
{
    switch (c[0])
    {
    case 'w':
    case 'W':
        return GameType::Wordle;
    case 'n':
    case 'N':
        return GameType::Nerdle;
    }
    return GameType::UnknowType;
}

int parse_options(int argc, char* argv[], GameType &type)
{
    // We need at least 2 options: The name of the function, and '-v'
    if (argc < 2)
    {
        return -EINVAL;
    }

    // We will handle opt errors ourselves
    opterr = 0;

    // Parse options
    for (;;)
    {
        switch (getopt(argc, argv, "hv:"))
        {
        // Unknown options 
        default:
        case '?':
        case 'h':
            return -EINVAL;
        // End of options
        case -1:
            break;
        
        // Select game type
        case 'v':
            if (optarg != NULL)
            {
                type = get_game_type(optarg);
                continue;
            }
            return -EINVAL;
        }
        break;
    }

    // One of the options was invalid
    if (optind != argc)
    {
        return -EINVAL;
    }
    return 0;
}

/**
 * Main function of LESolver
 *
 * @param[in] argc The number of options passed to the program
 * @param[in] argv The list of string options passed to the program
 * @return 0 on success, -ERROR on error (or '-h')
 */
int main(int argc, char* argv[])
{
    GameType type = GameType::UnknowType;
    int options_ret = parse_options(argc, argv, type);
    // Invalid argument or '-h'
    if (options_ret < 0)
    {
        usage(argv[0]);
        return options_ret;
    }

    // Start a game
    switch (type)
    {
    case GameType::Wordle:
        cout << "Initializing Wordle game" << endl;
        break;
    case GameType::Nerdle:
        cout << "Initializing Nerdle game" << endl;
        break;
    default:
        usage(argv[0]);
        return -EINVAL;
    }

    // Get guess map
    unordered_map<string, double> guesses_map;
    get_possible_guesses_map(type, guesses_map);

    string input_guess;
    // Select first guess
    input_guess = get_hardcoded_first_guess(type);
    cout << "Hardcoded best first guess: " << input_guess << endl;

    string input_hint;
    int hint_idx = -1;
    // Game loop
    for (;;)
    {
        cout 
            << "Please input your hint.\n"
            << "2 for correct location, 1 for correct character, 0 for unused."
            << endl;
        cin >> input_hint;

        hint_idx = hint_string_to_idx(game_guess_len_map[type], input_hint);
        while (hint_idx == -1)
        {
            cout << "That hint couldn't be parsed. Please re-enter the hint."
                << endl;
            cin >> input_hint;
            hint_idx = hint_string_to_idx(game_guess_len_map[type], 
                input_hint);
        }

        // Remove possible guesses that don't fit with the hint
        cull_solution_list(game_guess_len_map[type], guesses_map, 
            input_guess, hint_idx);

        // Get the next guess
        input_guess = get_best_guess(game_guess_len_map[type], guesses_map);
        if (guesses_map.size() == 1)
        {
            cout << "The solution is: " << input_guess << endl; 
            break;
        }
        else
        {
            cout << "Best guess: " << input_guess << endl; 
        }
    }

    return 0;
}
