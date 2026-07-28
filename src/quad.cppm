/**
 * @file quad.cppm
 * @brief A 3D quadrilateral geometry class
*/

module;

#include <memory>
#include <utility>
#include <cmath>

export module quad;

import vec3;
import hittable;
import interval;
import material;
import aabb;
import utils;
import hit_record;

/**
 * @class quad
 * @brief Represents a simple representation of a quadrilateral to render in a 3D space
 */
export class quad : public hittable {
    public:
        /**
         * @brief Constructs a new quad
         * @param Q The starting corner
         * @param u A vecctor representing the first side
         * @param v A vector representing the second side
         */
        constexpr quad(const point3& Q, const vec3& u, const vec3& v, std::shared_ptr<material> mat)
            : Q{Q}, u{u}, v{v}, mat{mat} {
                auto n = cross(u, v);
                normal = unit_vector(n);
                D = dot(normal, Q);
                w = n / dot(n, n);

                set_bounding_box();
            }

        /**
         * @brief Sets the bounding box of the quads
         */
        constexpr virtual void set_bounding_box() noexcept {
            auto bbox_diagonal1 = aabb{Q, Q + u + v};
            auto bbox_diagonal2 = aabb{Q + u, Q + v};
            bbox = aabb{bbox_diagonal1, bbox_diagonal2};
        }

        /**
         * @brief Returns the bounding box of the quad
         * 
         * @return The AABB box of the quad
         */
        [[nodiscard]]
        constexpr aabb bounding_box() const noexcept override {
            return bbox;
        }

        /**
         * @brief Returns if a ray hit the quad
         * 
         * @param r The ray vector
         * @param ray_t The valid interval [t_min, t_max] along the ray's trajectory where intersections are allowed to register.
         * @param rec The hit record used to store hit information if a hit occured
         * @return true if the ray vector hit the quad in the 3D space
         * @return false if the ray did not hit the quad in the 3D space
         */
        [[nodiscard]]
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            auto denom = dot(normal, r.direction());

            if (std::fabs(denom) < 1e-8)
                return false;

            auto t = (D - dot(normal, r.origin())) / denom;
            if (!ray_t.contains(t))
                return false;

            auto intersection = r.at(t);
            vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w, cross(u, planar_hitpt_vector));

            if (!is_interior(alpha, beta, rec))
                return false;

            rec.t = t;
            rec.p = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);

            return true;
        }

        /**
         * @brief Given the hit point in plane coordinates, return false if it is outside the primitive,
         otherwise set the hit record UV coordinates and return true
         * 
         * @param a First bound of the quad
         * @param b Second bound of the quad
         * @param rec The record containing all the information of the current hit
         * @return true If the hit point is inside the primitive 
         * @return false If the hit point is outside the primitive
         */
        virtual constexpr bool is_interior(double a, double b, hit_record& rec) const noexcept {
            interval unit_interval = interval{0, 1};

            if (!unit_interval.contains(a) || !unit_interval.contains(b))
                return false;
        
            rec.u = a;
            rec.v = b;
            return true;
        }


    
    private:
        point3 Q;
        vec3 u, v, w, normal;
        std::shared_ptr<material> mat{nullptr};
        aabb bbox;
        double D{0.0};
};