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
         * @param origin the origin of the ray
         * @param direction the direction of the ray
         */
        constexpr ray(const point3& origin, const vec3& direction) noexcept : orig{origin}, dir{direction} {}

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
};
