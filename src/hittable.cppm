/**
 * @file hittable.cppm
 * @brief Interface to represent an object that can be hit by a ray
 */

module;

#include <memory>

export module hittable;

class material;
import vec3;
import ray;
import interval;
import hit_record;
import aabb;

/**
 * @class hittable
 * @brief Represents an object that can be hit by light ray
 * @note This class is **non-copyable** to prevent object slicing of derived geometries (like spheres or triangles) when managed polymorphically. 
 *       Move semantics are supported.
 */
export class hittable {
    public:
        /**
         * @brief Destroy the hittable object
         */
        virtual ~hittable() = default;

        hittable(const hittable&) = delete;

        hittable& operator=(const hittable&) = delete;

        hittable(hittable&&) noexcept = default;
        hittable& operator=(hittable&&) noexcept = default;

        /**
         * @brief Returns if a ray hit the  object
         * 
         * @param r The ray vector
         * @param ray_t The valid interval [t_min, t_max] along the ray's trajectory where intersections are allowed to register.
         * @param rec The hit record used to store hit information if a hit occured
         * @return true if the ray vector hit the object in the 3D space
         * @return false if the ray did not hit the object in the 3D space
         */
        [[nodiscard]]
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

        [[nodiscard]]
        virtual aabb bounding_box() const noexcept = 0;

    protected:
        constexpr hittable() noexcept = default;
};