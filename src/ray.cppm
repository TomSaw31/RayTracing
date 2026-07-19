/**
 * @file ray.cppm
 * @brief A 3D ray vector class
*/

export module ray;

import vec3;

/**
 * @class ray
 * @brief Represents a light ray in a 3D space
 * 
 */
export class ray {
    public:
        /**
         * @brief Returns a new ray
         */
        constexpr ray() noexcept {};

        /**
         * @brief Returns a new ray
         * 
         * @param origin The origin of the ray
         * @param direction The direction of the ray
         * @param tm The instant during which the ray hit
         */
        constexpr ray(const point3& origin, const vec3& direction, double time) noexcept : orig{origin}, dir{direction}, tm{time} {}

        /**
         * @brief Returns a new ray
         * 
         * @param origin The origin of the ray 
         * @param direction The direction of the ray
         */
        constexpr ray(const point3& origin, const vec3& direction) : ray{origin, direction, 0} {}

        /**
         * @brief Returns the origin component of the ray
         * 
         * @return The origin component of the ray
         */
        [[nodiscard]]
        constexpr const point3& origin() const noexcept {
            return orig;
        }

        /**
         * @brief Returns the direction component of the ray
         * 
         * @return The direction component of the ray
         */
        [[nodiscard]]
        constexpr const vec3& direction() const noexcept {
            return dir;
        }

        /**
         * @brief Returns the time component of the ray
         * 
         * @return The time component of the ray
         */
        [[nodiscard]]
        constexpr const double time() const {
            return tm;
        }

        /**
         * @brief Returns the parametric position of a ray along its trajectory
         * 
         * @param t The parameters that caracterize the position along the ray's trajectory
         * @return constexpr point3 
         */
        [[nodiscard]]
        constexpr point3 at(double t) const noexcept {
            return orig + t * dir;
        }
    
    private:
        point3 orig{};
        vec3 dir{};
        double tm;
};
