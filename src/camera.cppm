/**
 * @file camera.cppm
 * @brief A 3D camera class for rendering
 */

module;

#include <iostream>
#include <cmath>
#include <print>
#include <vector>
#include <execution>
#include <numeric>
#include <mutex>

export module camera;

import color;
import hittable;
import vec3;
import ray;
import hit_record;
import material;
import random;
import utils;

/**
 * @class camera
 * @brief Represents the camera (i.e the rendering viewpoint) and supports basic rendering operations
 */
export class camera {
    public:
        double aspect_ratio{1.0};
        int image_width{100};
        int samples_per_pixel{10};
        int max_depth{10};

        double vfov{90};
        point3 lookfrom{0, 0, 0};
        point3 lookat{0, 0, -1};
        vec3 vup{0, 1, 0};

        double defocus_angle{0};
        double focus_dist{10};

        /**
         * @brief Renders an image and outputs the result as a PPM format file 
         * 
         * @param world The world/scene to render
         */
        void render(const hittable& world) {    
            initialize();

            std::vector<color> image_buffer(image_width * image_height);
            std::vector<int> line_indices(image_height);
            std::iota(line_indices.begin(), line_indices.end(), 0);

            std::atomic<int> lines_remaining{image_height};
            std::mutex clog_mutex;

            std::for_each(std::execution::par, line_indices.begin(), line_indices.end(), [&](int j) {
                for(int i = 0; i < image_width; ++i) {
                    color pixel_color(0, 0, 0);
                    for(int sample = 0; sample < samples_per_pixel; ++sample) {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    image_buffer[j * image_width + i] = pixel_samples_scale * pixel_color;
                }
                int remaining = --lines_remaining;
                {
                    std::lock_guard<std::mutex> lock(clog_mutex);
                    std::clog << "\rScanlines remaining: " << remaining << "    " << std::flush;
                }
            });
            std::println(std::cout, "P3\n{} {}\n255", image_width, image_height);

            for(int j = 0; j < image_height; ++j) {
                for(int i = 0; i < image_width; ++i) {
                    write_color(std::cout, image_buffer[j * image_width + i]);
                }
            }
    
            std::clog << "\nDone.\n";
        }
    
    private:
        int image_height;
        double pixel_samples_scale;
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        vec3 u, v, w;
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;

        /**
         * @brief Initializes the camera's properties
         */
        void initialize() {
            image_height = static_cast<int>(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta / 2.0);
            auto viewport_height = 2.0 * h * focus_dist;
            auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
            
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            vec3 viewport_u = viewport_width * u;
            vec3 viewport_v = viewport_height * -v;

            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            const auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2.0 - viewport_v / 2.0;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            const auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        /**
         * @brief Calculates the color of a surface hit by a light ray
         * 
         * @param r The incoming ray
         * @param depth The depth of the bouncing ray from the hit surface
         * @param world The world to render
         * @return The calculated color from the multiple ray hit
         */
        [[nodiscard]]
        color ray_color(const ray& r, int depth, const hittable& world) const {
            if (depth <= 0) {
                return color{0.0, 0.0, 0.0};
            }
            hit_record rec;
            if(world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, depth - 1, world);
                }
                return color{0.0, 0.0, 0.0};
            }
            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * color{1.0, 1.0, 1.0} + a * color{0.5, 0.7, 1.0};
        }
        
        /**
         * @brief Generates a ray for a given pixel coordinates with anti-aliasing and depth-of-field sampling
         * 
         * @param i The horizontal pixel index
         * @param j The vertical pixel index
         * @return A ray object created from the camera/defocus disk and shooting through the sampled pixel location
         */
        [[nodiscard]]
        ray get_ray(int i, int j) const {
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = (defocus_angle <= 0.0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;
            auto ray_time = random_double();

            return ray{ray_origin, ray_direction, ray_time};
        }

        /**
         * @brief Returns a random vector placed in the center of a pixel on the XY plane
         * 
         * @return The random vector pointing to the center of a random pixel
         */
        [[nodiscard]]
        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
        }

        /**
         * @brief Return a random point according to the defocus values
         * 
         * @return A defocused point around the center point
         */
        [[nodiscard]]
        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }
};