/**
 * @file random.cppm
 * @brief A module used to create efficient randomness based on the Mersenne Twister algorithm
*/

module;

#include <random>

export module random;

namespace {inline std::mt19937 generator{std::random_device{}()};}

/**
 * @brief Returns a random double between 0 and 1
 * 
 * @return The random double between 0 and 1
 */
export double random_double() {
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(generator);
}

/**
 * @brief Returns a random double in a range
 * 
 * @param min The lower bound of the range
 * @param max The upper bound of the range
 * @return The new random number included in the given range
 */
export double random_double(double min, double max) {
    return min + (max - min) * random_double();
}