/**
 * @file material.cppm
 * @brief A material class used for render
*/

module;

#include <cmath>

export module material;

import color;
import vec3;
import ray;
import hit_record;
import random;
import utils;

/**
 * @class material
 * @brief Represents a material used on a geometry object for render
 *
 * Defines how light interacts with geometry surfaces via scattering, absorption and ray generation.
 */
export class material {
    public:
        /**
         * @brief Destroy the material object
         */
        virtual ~material() = default;

        /**
         * @brief Determines how an incoming ray scatters when hitting a surface
         * 
         * @param r_in The incoming ray that intersects the geometry
         * @param rec Structure containing surface information at the hit point
         * @param attenuation Output parameter describing how much color is absorbed or reflected
         * @param scattered Output parameter assigned the newly generated bounced ray direction
         * @return true if the material successfully scatters light
         * @return false if the light is completely absorbed by the surface
         */
        [[nodiscard]]
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
            return false;
        }
};

/**
 * @class lambertian
 * @brief Simulates a diffuse surface
 * 
 * Uses a Lambertian distribution where light scatters randomly in any direction across the hemisphere centered on the surface normal
 */
export class lambertian : public material {
    public:
        /**
         * @brief Constructs a Lambertian material with a base color
         *
         * @param albedo The color of the surface
         */
        explicit lambertian(const color& albedo) noexcept : albedo{albedo} {}

        [[nodiscard]]
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_direction = rec.normal + random_unit_vector();
            
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
    
    private:
        color albedo;

};

/**
 * @class metal
 * @brief Simulates a metallic surface
 * 
 * reflections can be blurred by modifying the fuzz parameter to mimic metal textures
 */
export class metal : public material {
    public:
        /**
         * @brief Constructs a metal surface
         *
         * @param albedo The surface color
         * @param fuzz Polishing roughness parameter bounded between [0, 1]
                       0 gives a perfect mirror; 1 gives highly blurred reflections.
         */
        explicit metal(const color& albedo, double fuzz) : albedo{albedo}, fuzz{fuzz < 1 ? fuzz : 1} {}

        [[nodiscard]]
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0.0);
        }
    
    private:
        color albedo;
        double fuzz;
};

/**
 * @class dielectric
 * @brief Simulates transparent glass-like materials
 */
export class dielectric : public material {
    public:
        /**
         * @brief Constructs a dielectric boundary
         *
         * @param refraction_index Index of refraction
         */
        explicit dielectric(double refraction_index) noexcept : refraction_index{refraction_index} {}

        [[nodiscard]]
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
                direction = reflect(unit_direction, rec.normal);
            } else {
                direction = refract(unit_direction, rec.normal, ri);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        double refraction_index;

        /**
         * @brief Uses Schlick's approximation to compute glass reflectivity variation by viewpoint angle
         *
         * @param cosine Angle cosine between the incoming ray direction and the surface normal
         * @param refraction_index The relative index of refraction across the medium boundary
         * @return The probability value [0, 1] of the ray choosing reflection over refraction
         */
        [[nodiscard]]
        static double reflectance(double cosine, double refraction_index) noexcept {
            auto r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
            r0 = r0 * r0;
            return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5);
        }
};
