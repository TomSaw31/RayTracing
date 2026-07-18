/**
 * @file interval.cppm
 * @brief A class for math related interval operations
*/

export module interval;

import utils;

/**
 * @class interval
 * @brief Represents a real-valued interval that supports standard interval operations
 */
export class interval {
    public: 
        double min{infinity};
        double max{-infinity};

        /**
         * @brief Creates a default interval
         */
        constexpr interval() noexcept = default;

        /**
         * @brief Creates an interval between the min and max
         * 
         * @param min The lower bound of the interval
         * @param max The upper bound of the interval
         */
        constexpr interval(double min, double max) noexcept : min{min}, max{max} {}

        /**
         * @brief Returns the length/size of the interval
         * 
         * @return The length/size of the interval
         */
        [[nodiscard]]
        constexpr double size() const noexcept {
            return max - min;
        }

        /**
         * @brief Checks if a value is in the closure of the interval
         * 
         * @param x The value to check
         * @return true if the value x is in the interval
         * @return false if the value x is not in the interval
         */
        [[nodiscard]]
        constexpr bool contains(double x) const noexcept {
            return min <= x && x <= max;
        }

        /**
         * @brief Check if a value is in the interior of the interval
         * 
         * @param x The value to check
         * @return true if the value x is in the interval
         * @return false if the value x is not in the interval
         */
        [[nodiscard]]
        constexpr bool surrounds(double x) const noexcept {
            return min < x && x < max;
        }

        /**
         * @brief Clamps the given value in the interval
                  If the value is lower than the lower bound it is replaced by the lower bound
                  If the value is higher than the upper bound it is replaced by the upper bound
         * 
         * @param x The value to clamp
         * @return The value included in the interval
         */
        [[nodiscard]]
        constexpr double clamp(double x) const noexcept {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }
        
        static const interval empty, universe;
};

const interval interval::empty{infinity, -infinity};
const interval interval::universe{-infinity, infinity};