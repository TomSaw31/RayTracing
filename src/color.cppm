/**
 * @file color.cppm
 * @brief Multiple functions for color handling during rendering
 */

module;

#include <cmath>
#include <iostream>
#include <print>

export module color;

import vec3;
import interval;

/**
 * @brief alias for a vec3 that represents a color
 */
export using color = vec3;

/**
 * @brief Converts color from linear color space to gamma color space
 * 
 * @param linear_component The linear component of the color
 * @return The gamma component of the color calculated from the linear component
 */
export [[nodiscard]]
constexpr double linear_to_gamma(double linear_component) noexcept {
    if (linear_component > 0.0) {
        return std::sqrt(linear_component);
    }
    return 0.0;
}

/**
 * @brief Writes the given color in the given output file. Converts the pixel color from linear space color to gamma space color
 * 
 * @param out The output file to put the calculated color in
 * @param pixel_color The original pixel color in linear color space
 */
export void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static constexpr interval intensity(0.000, 0.999);
    const auto rbyte = static_cast<int>(256 * intensity.clamp(r));
    const auto gbyte = static_cast<int>(256 * intensity.clamp(g));
    const auto bbyte = static_cast<int>(256 * intensity.clamp(b));

    std::println(out, "{} {} {}", rbyte, gbyte, bbyte);
}