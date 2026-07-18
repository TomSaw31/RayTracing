/**
 * @file vec3.cppm
 * @brief A 3D vector class for linear algebra operations
*/

module;

#include <iostream>
#include <array>

export module vec3;

import random;
import utils;

/**
 * @class vec3
 * @brief Represents a 3D vector or point in space.
 * 
 * Supports standard vector operations.
 */
export class vec3 {
    public:
        std::array<double,3> e{};

        /**
        * @brief Constructs a new vector.
        */
        constexpr vec3() noexcept = default;

        /**
        * @brief Constructs a new vector.
        *
        * @param e0 X component
        * @param e1 Y component
        * @param e2 Z component
        */
        constexpr vec3(double e0, double e1, double e2) noexcept : e{e0, e1, e2} {}

        /**
        * @brief Returns the X component
        */
        [[discard]]
        constexpr double x() const noexcept {return e[0];}

        /**
        * @brief Returns the Y component
        */
        [[discard]]
        constexpr double y() const noexcept {return e[1];}

        /**
        * @brief Returns the Z component
        */
        [[discard]]
        constexpr double z() const noexcept {return e[2];}

        /**
         * @brief Unary minus operator. Negates all components of the vector
         *
         * @return A new vec3 pointing in the opposite direction
         */
        [[nodiscard]]
        constexpr vec3 operator-() const noexcept {
            return vec3(-e[0],-e[1],-e[2]);
        }

        /**
         * @brief Subscript operator for coordinate read access
         *
         * @param i Component index (0 for X, 1 for Y, 2 for Z)
         * @return The value of the requested component
         */
        [[nodiscard]]
        constexpr double operator[](std::size_t i) const noexcept {
            return e[i];
        } 

        /**
         * @brief Subscript operator for coordinate write access
         *
         * @param i Component index (0 for X, 1 for Y, 2 for Z)
         * @return A reference to the requested component
         */
        [[nodiscard]]
        constexpr double& operator[](std::size_t i) noexcept {
            return e[i];
        }

        /**
         * @brief Performs an addition component-wise
         * 
         * @param v The other vector to perform the addition
         * @return The result of the addition
         */
        constexpr vec3& operator+=(const vec3& v) noexcept {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        /**
         * @brief Performs component-wise scalar multiplication
         * 
         * @param c The scalar to perform the multiplication
         * @return The result of the multiplication
         */
        constexpr vec3& operator*=(double c) noexcept {
            e[0] *= c;
            e[1] *= c;
            e[2] *= c;
            return *this;
        }
    
        /**
         * @brief Performs component-wise scalar division
         * 
         * @param c The scalar to perform the division
         * @return The result of the division
         */
        constexpr vec3& operator/=(double c) noexcept {
            return *this *= 1/c;
        }   

        /**
         * @brief Returns the length of the vector
         * 
         * @return The length of the vector
         */
        [[nodiscard]]
        double length() const noexcept {
            return std::sqrt(length_squared());
        }

        /**
         * @brief Returns the squared length of the vector
         * 
         * @return The squared length of the vector
         */
        [[nodiscard]]
        constexpr double length_squared() const noexcept {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        /**
         * @brief Returns a new vector with random coordinates
         * 
         * @return The new random vector
         */
        [[nodiscard]]
        static vec3 random() {
            return vec3{random_double(), random_double(), random_double()};
        }   

        /**
         * @brief Returns a new vector with random coordinates included in a certain range
         * 
         * @param min The minimum of the random coordinates
         * @param max The maximum of the random coordinates
         * @return The new random vector
         */
        [[nodiscard]]
        static vec3 random(double min, double max) {
            return vec3{random_double(min, max), random_double(min, max), random_double(min, max)};
        }

        /**
         * @brief Checks if the vector is close to zero in all dimensions
         * 
         * @return true if all components are less than a small epsilon value
         * @return false if at least on component is more than a small espilon value 
         */
        [[nodiscard]]
        bool near_zero() const {
            return (std::fabs(e[0]) < epsilon) && (std::fabs(e[1]) < epsilon) && (std::fabs(e[2]) < epsilon);
        }

};

/**
 * @brief alias for a vec3 that represents a position in space
 */
export using point3 = vec3;

/**
 * @brief Formats the vector to a standard text output stream (Outputs format: "x y z")
 */
export std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

/**
 * @brief Performs a component-wise addition between two vectors
 * 
 * @param u The first vector to perform the addition
 * @param v The second vector to perform the addition
 * @return The result of the addition
 */
export constexpr vec3 operator+(const vec3& u, const vec3& v) noexcept {
    return vec3{u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

/**
 * @brief Performs a component-wise substraction between two vectors
 * 
 * @param u The first vector to perform the substraction
 * @param v The second vector to perform the substraction
 * @return The result of the substraction
 */
export constexpr vec3 operator-(const vec3& u, const vec3& v) noexcept {
    return vec3{u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

/**
 * @brief Performs a component-wise multiplication between two vectors
 * 
 * @param u The first vector to perform the multiplication
 * @param v The second vector to perform the multiplication
 * @return The result of the multiplication
 */
export constexpr vec3 operator*(const vec3& u, const vec3& v) noexcept {
    return vec3{u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

/**
 * @brief Performs a component-wise scalar multiplication
 * 
 * @param c The scalar to perform the scalar multiplication
 * @param u The vector to perform the scalar multiplication
 * @return The result of the multiplication
 */
export constexpr vec3 operator*(const double c, const vec3& u) noexcept {
    return vec3{c * u.e[0], c * u.e[1], c * u.e[2]};
}

/**
 * @brief Performs a component-wise scalar multiplication
 * 
 * @param u The vector to perform the scalar multiplication
 * @param c The scalar to perform the scalar multiplication
 * @return The result of the multiplication
 */
export constexpr vec3 operator*(const vec3& u, double c) noexcept {
    return c * u;
}

/**
 * @brief Performs a component-wise scalar division
 * 
 * @param u The vector to perform the scalar division
 * @param c The scalar to perform the scalar division
 * @return The result of the division
 */
export constexpr vec3 operator/(const vec3& u, double c) noexcept {
    return (1/c) * u;
}

/**
 * @brief Performs a dot product between two vectors
 * 
 * @param u The first vector to perform the dot product
 * @param v The second vector to perform the dot product
 * @return The result of the dot product
 */
export constexpr double dot(const vec3& u, const vec3& v) noexcept {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

/**
 * @brief Performs a cross product between two vectors
 * 
 * @param u The first vector to perform the cross product
 * @param v The second vector to perform the cross product
 * @return The result of the cross product
 */
export constexpr vec3 cross(const vec3& u, const vec3& v) noexcept {
    return vec3{u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]};
}

/**
 * @brief Normalize and return a unit vector
 * 
 * @param v The vector to normalize
 * @return The unit vector of the one given
 */
export vec3 unit_vector(const vec3& v) noexcept {
    return v / v.length();
}

/**
 * @brief Returns a random unit vector
 * 
 * @return The new random unit vector
 */
export vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1, 1);
        auto lensq = p.length_squared();
        if (small_epsilon < lensq && lensq <= 1) {
            return p / std::sqrt(lensq);
        }
    }
}

/**
 * @brief Returns a new random unit vector in the unit disk
 * 
 * @return The new random unit vector
 */
export vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3{std::rand() / (RAND_MAX + 1.0), std::rand() / (RAND_MAX + 1.0), 0};
        if (p.length_squared() < 1) {
            return p;
        }
    }
}

/**
 * @brief Returns a new random unit vector on a hemisphere according to the normal given
 * 
 * @param normal The normal defining the hemisphere
 * @return The new random unit vector in the calculated hemisphere
 */
export vec3 random_on_hemisphere(const vec3& normal) {
    const auto on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) {
        return on_unit_sphere;
    } else {
        return -on_unit_sphere;
    }
}

/**
 * @brief Calculates the reflection vector
 * 
 * @param v The incoming ray vector.
 * @param n The surface normal vector
 * @return The reflected ray vector direction
 */
export constexpr vec3 reflect(const vec3& v, const vec3& n) noexcept {
    return v - 2 * dot(v, n) * n;
}

/**
 * @brief Evaluates Snell's Law to calculate the light refraction vector
 * 
 * @param uv  The unit vector representing the incoming ray 
 * @param n The surface normal vector
 * @param etai_over_etat The ratio of refractive indices
 * @return The refracted ray vector direction
 */
export vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    const auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    const vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    const vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}
