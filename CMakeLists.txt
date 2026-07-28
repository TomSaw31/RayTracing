cmake_minimum_required(VERSION 3.28)
project(RayTracing VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(RayTracing 
    src/main.cpp
)

target_sources(RayTracing
    PUBLIC
        FILE_SET CXX_MODULES TYPE CXX_MODULES FILES
            src/color.cppm
            src/camera.cppm
            src/hittable_list.cppm
            src/hittable.cppm
            src/hit_record.cppm
            src/interval.cppm
            src/material.cppm
            src/ray.cppm
            src/sphere.cppm
            src/vec3.cppm
            src/random.cppm
            src/utils.cppm
            src/aabb.cppm
            src/bvh.cppm
            src/texture.cppm
            src/image.cppm
            src/perlin.cppm
            src/quad.cppm
)

target_include_directories(RayTracing PRIVATE include/raytracer)