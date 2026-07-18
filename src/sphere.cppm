/**
 * @file sphere.cppm
 * @brief A 3D sphere gemoetry class
*/

module;

#include <memory>
#include <utility>

export module sphere;

import vec3;
import hittable;
import interval;
import material;

/**
 * @class sphere
 * @brief Represents a simple representation of a sphere to render in a 3D space
 * 
 */
export class sphere : public hittable {
    public:
        /**
         * @brief Constructs a new sphere
         *
         * @param center The center of the sphere
         * @param radius The radius of the sphere
         * @param mat the material used to render the sphere
         */
        constexpr sphere(const point3& center, double radius, std::shared_ptr<material> mat) noexcept : center{center}, radius{std::max(0.0, radius)}, mat{std::move(mat)} {}

        /**
         * @brief Returns if a ray hit the sphere
         * 
         * @param r The ray vector
         * @param ray_t The valid interval [t_min, t_max] along the ray's trajectory where intersections are allowed to register.
         * @param rec The hit record used to store hit information if a hit occured
         * @return true if the ray vector hit the sphere in the 3D space
         * @return false if the ray did not hit the sphere in the 3D space
         */
        [[nodiscard]]
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            auto a  = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius * radius;

            auto discriminant = h * h - a * c;
            if (discriminant < 0) {
                return false;
            }
            auto sqrtd = sqrt(discriminant);

            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

    private:
        point3 center;
        double radius;
        std::shared_ptr<material> mat;
};