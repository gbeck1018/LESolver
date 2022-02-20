#include "solve.hpp"

#include <vector>
using std::vector;

#include <cmath>
using std::pow;

/**
 * Calculate the hint given for a given guess and solution and return it as
 * an index into our buckets vector.
 *
 * @param[in] guess_len The guess length
 * @param[in] guess The guess to check
 * @param[in] solution The solution to check against
 * @return the bucket idx of the guess
 */
int get_guess_hint_idx(int guess_len, string const& guess, 
    string const& solution)
{
    int ret = 0;
    // We will keep track of whick indexes we've already used in a bitmap
    int used_bitmap = 0;
    
    for (int i = 0; i < guess_len; ++i)
    {
        // First, check if this idx should be green
        if (guess[i] == solution[i])
        {
            ret += (2 * pow(NUM_OUTCOMES, i));
            used_bitmap |= (1 << i);
            continue;
        }
        // If not green, check if it can be yellow
        for (int j = 0; j < guess_len; ++j)
        {
            // Skip if this idx is used
            if (!(used_bitmap & (1 << j)) && guess[i] == solution[j])
            {
                // We need to double check if this new 
                // idx should be green instead of yellow
                if (guess[j] == solution[j])
                {
                    continue;
                }
                ret += (1 * pow(NUM_OUTCOMES, i));
                used_bitmap |= (1 << j);
                break;
            }
        }
    }
    return ret;
}

/**
 * Convert a hint string to an solution bucket index
 *
 * @param[in] guess_len The guess length
 * @param[in] hint The hint string. 2 for Green, 1 for Yellow, 0 for black
 * @return The index, -1 if the hint is not in a valid format.
 */
int hint_string_to_idx(int guess_len, string const& hint)
{
    if ((int)hint.size() != guess_len)
    {
        return -1;
    }
    int ret = 0;
    // For each character, convert it to part of the solution bucket index
    for (int i = 0; i < guess_len; ++i)
    {
        switch (hint[i])
        {
        case '2':
            ret += (2 * pow(NUM_OUTCOMES, i));
            continue;
        case '1':
            ret += (1 * pow(NUM_OUTCOMES, i));
            continue;
        case '0':
            continue;
        default:
            // Invalid character
            return -1;
            continue;
        }
    }
    return ret;
}

/**
 * Get the entropy of one guess within a set of solutions
 *
 * @param[in] guess_len The guess length
 * @param[in] guess The guess to return the entropy of
 * @param[in] guesses_map The map of guesses
 * @return The entropy of the guess
 */
double entropy_of_guess(int guess_len, string const& guess, 
    unordered_map<string, double> const& guesses_map)
{
    vector<double> buckets(pow(NUM_OUTCOMES, guess_len), 0);
    double entropy = 0;
    // Fill buckets with the distribution of answersxccdxz
    for (auto const& [possible_solution, entropy]: guesses_map)
    {
        ++buckets[get_guess_hint_idx(guess_len, guess, possible_solution)];
    }
    // For each buckets, add its entropy contribution
    for (int i = 0; i < pow(NUM_OUTCOMES, guess_len); ++i)
    {
        if (buckets[i] > 0)
        {
            // Log2(1) = -1 in this specific case as a slight bias to getting
            // the answer, rather than just lowering entropy. This bucket
            // represents all greens.
            if (i == pow(NUM_OUTCOMES, guess_len) - 1)
            {
                entropy += (buckets[i] * -1);
            }
            else
            {
                entropy += (buckets[i] * log2(buckets[i]));
            }
        }
    }
    return (entropy/guesses_map.size());
}

/**
 * Test if a given guess would produce a given hint
 *
 * @param[in] guess_len The guess length
 * @param[in] solution The solution to compare against
 * @param[in] guess The guess which produced the hint
 * @param[in] hint The hint idx that the guess produced
 * @return True if the given guess would produce the given hint
 */
bool does_solution_fit_hint(int guess_len, string const& solution, 
    string const& guess, int hint)
{
    return hint == get_guess_hint_idx(guess_len, guess, solution);
}

/**
 * Remove guesses that don't match the hint
 *
 * @param[in] guess_len The guess length
 * @param[in, out] guesses_map A map where the keys are the set of guesses,
 *  We will be removing members.
 * @param[in] guess The guess that produced this hint
 * @param[in] hint The hint produced by this guess
 */
void cull_solution_list(int guess_len, unordered_map<string, 
    double>& guesses_map, string const& guess, int hint)
{
    // Iterator loop, beacuse we will be removing members
    for (auto it = guesses_map.begin(); it != guesses_map.end(); )
    {
        if (!does_solution_fit_hint(guess_len, it->first, guess, hint))
        {
            // 'it' gets the next iterator, stays valid
            it = guesses_map.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

/**
 * Get the best guess from a set of possible guesses
 *
 * @param[in] guess_len The guess length
 * @param[in, out] guesses_map A map where the keys are the set of guesses,
 *  The values will be modified to be the entropies of the guesses
 * @return The string with the best guess
 */
string get_best_guess(int guess_len, 
    unordered_map<string, double>& guesses_map)
{
    double best_entropy = UNINITIALIZED_ENTROPY;
    string best_guess;
    for (auto &[key, val]: guesses_map)
    {
        val = entropy_of_guess(guess_len, key, guesses_map);
        if (val < best_entropy)
        {
            best_entropy = val;
            best_guess = key;
        }
    }
    return best_guess;
}