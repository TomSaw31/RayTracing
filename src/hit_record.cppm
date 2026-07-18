/**
 * @file hit_record.cppm 
 * @brief Data structure used to store a hit between a ray and a hittable object
 */

module;

#include <memory>

export module hit_record;

export class material;
import vec3;
import ray;

/**
 * @struct hit_record
 * @brief Stores a hit created between a hittable surface and a ray
 */
export struct hit_record {
    public:
        point3 p{};
        vec3 normal{};
        std::shared_ptr<const material> mat{nullptr};
        double t{0.0};
        bool front_face{false};

        /**
         * @brief Sets the face normal of the hit object
         * 
         * @param r The incoming ray
         * @param outward_normal The outward normal
         */
        constexpr void set_face_normal(const ray& r, const vec3& outward_normal) noexcept {
            front_face = dot(r.direction(), outward_normal) < 0.0;
            normal = front_face ? outward_normal : -outward_normal;
        }   
};