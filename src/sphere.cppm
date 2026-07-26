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
import aabb;
import utils;

/**
 * @class sphere
 * @brief Represents a simple representation of a sphere to render in a 3D space
 * 
 */
export class sphere : public hittable {
    public:
        /**
         * @brief Constructs a new stationary sphere
         *
         * @param static_center The center of the sphere
         * @param radius The radius of the sphere
         * @param mat the material used to render the sphere
         */
        constexpr sphere(const point3& static_center, double radius, std::shared_ptr<material> mat) noexcept : center{static_center, vec3{0,0,0}}, radius{std::max(0.0, radius)}, mat{std::move(mat)} {
            auto rvec = vec3(radius, radius, radius);
            bbox = aabb{static_center - rvec, static_center + rvec};
        }

        /**
         * @brief Constructs a new moving sphere
         *
         * @param center1 The initial center position of the sphere
         * @param center2 The end center position of the sphere
         * @param radius The radius of the sphere
         * @param mat the material used to render the sphere
         */
        constexpr sphere(const point3& center1, const point3& center2, double radius, std::shared_ptr<material> mat) noexcept : center{center1, center2 - center1}, radius{std::max(0.0, radius)}, mat{std::move(mat)} {
            auto rvec = vec3{radius, radius, radius};
            aabb box1{center.at(0) - rvec, center.at(0) + rvec};
            aabb box2{center.at(1) - rvec, center.at(1) + rvec};
            bbox = aabb{box1, box2};
        }

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
            point3 current_center = center.at(r.time());
            vec3 oc = current_center - r.origin();
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
            vec3 outward_normal = (rec.p - current_center) / radius;
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv(outward_normal, rec.u, rec.v);
            rec.mat = mat;

            return true;
        }

        [[nodiscard]]
        constexpr aabb bounding_box() const noexcept override {
            return bbox;
        }

    private:
        ray center;
        double radius;
        std::shared_ptr<material> mat;
        aabb bbox;

        /**
         * @brief Computes the coordinates u and v from a point on the unit sphere centered at the origin
         * 
         * @param p A given point on the sphere of radius one, centered at the origin
         * @param u Returned value in [0,1] of angle around the Y axis from X = -1
         * @param v Returned value in [0,1] of angle from Y = -1 to Y = +1
         */
        static void get_sphere_uv(const point3& p, double& u, double& v) {
            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + pi;
            u = phi / (2 * pi);
            v = theta / pi;
        }
};