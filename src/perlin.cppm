/**
* @file perlin.cppm
* @brief A class used to generate perlin noise 
*/
module;

#include <cmath>

export module perlin;

import vec3;
import color;
import random;

/**
 * @class perlin
 * @brief A class used to generate perlin noise
 */
export class perlin {
public:
    /**
     * @brief Constructs a random perlin noise pattern
     * 
     */
    constexpr perlin() noexcept {
        for(int i = 0; i < point_count; ++i) {
            randvec[i] = unit_vector(vec3::random(-1,1));
        }

        perlin_generate_perm(perm_x);
        perlin_generate_perm(perm_y);
        perlin_generate_perm(perm_z);
    }

    /**
     * @brief Generates random noise color at the given point
     * 
     * @param p The point to generate noise on
     * @return The random perlin noise
     */
    [[nodiscard]]
    constexpr double noise(const point3& p) const noexcept {
        auto u = p.x() - std::floor(p.x());
        auto v = p.y() - std::floor(p.y());
        auto w = p.z() - std::floor(p.z());

        auto i = static_cast<int>(std::floor(p.x()));
        auto j = static_cast<int>(std::floor(p.y()));
        auto k = static_cast<int>(std::floor(p.z()));
        vec3 c[2][2][2];

        for(int di = 0; di < 2; ++di)
            for(int dj = 0; dj < 2; ++dj)
                for(int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = randvec[
                        perm_x[(i + di) & 255] ^
                        perm_y[(j + dj) & 255] ^
                        perm_z[(k + dk) & 255]
                    ];
        return perlin_interp(c, u, v, w);
    }

    /**
     * @brief Generates turbulence at a given point. Turbulence is the sum of repeated calls to noise
     * 
     * @param p The point to generate turbulence on
     * @param depth The depth of the turbulence
     * @return The turbulence value
     */
    [[nodiscard]]
    constepxr double turb(const point3& p, int depth) const noexcept {
        auto acc = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for(int i = 0; i < depth; ++i) {
            acc += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return std::fabs(acc);
    }

private:
    static const int point_count{256};
    vec3 randvec[point_count];
    int perm_x[point_count];
    int perm_y[point_count];
    int perm_z[point_count];

    /**
     * @brief Generates permutations to create randomness
     * 
     * @param p The point to scramble
     */
    static void perlin_generate_perm(int * p) noexcept {
        for(int i = 0; i < point_count; ++i) {
            p[i] = i;
        }
        permute(p, point_count);
    }

    /**
     * @brief Permutes the given array p, n times
     * 
     * @param p The array to shuffle
     * @param n The amount of shuffles to do
     */
    static void permute(int * p, int n) noexcept {
        for(int i = n - 1; i > 0; --i) {
            int target = random_int(0,i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    /**
     * @brief Interpolates the perlin noise
     * 
     * @param c The array containing permutations on the 3 axis
     * @param u The first texture coordinate
     * @param v The second texture coordinate
     * @param w The third texture coordinate
     * @return The results of the perlin noise interpolation
     */
    [[nodiscard]]
    static double perlin_interp(const vec3 c[2][2][2], double u, double v, double w) noexcept {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        auto acc = 0.0;

        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k) {
                    vec3 weight_v(u - i, v - j, w - k);
                    acc += 
                        (i * uu + (1 - i) * (1 - uu)) *
                        (j * vv + (1 - j) * (1 - vv)) * 
                        (k * ww + (1 - k) * (1 - ww)) *
                        dot(c[i][j][k], weight_v);
                }

        return acc;
    }
};