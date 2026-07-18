/**
 * @file utils.cppm
 * @brief Some useful math functions and constants
*/

module;

#include <cmath>
#include <limits>
#include <numbers>

export module utils;

/**
 * @brief Constant representing the infinity to use in math intervals
 */
export inline constexpr double infinity = std::numeric_limits<double>::infinity();

/**
 * @brief Pi constant to convert angles between radians and degrees
 */
export inline constexpr double pi = std::numbers::pi;

/**
 * @brief Constant to check if a value is near zero
 */
export inline constexpr double epsilon = 1e-8;
/**
 * @brief Very precise constant to check if a value is near zero
 */
export inline constexpr double small_epsilon = 1e-160;

/**
 * @brief Converts angle in degrees to radians
 * 
 * @param degrees The angle in degrees to convert
 * @return The angle in radians
 */
export [[nodiscard]]
constexpr double degrees_to_radians(double degrees) noexcept {
    return degrees * pi / 180.0;
}