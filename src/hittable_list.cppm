/**
 * @file hittable_list.cppm
 * @brief A container class used to stores hittable objects
 */

module;

#include <vector>
#include <cmath>
#include <memory>

export module hittable_list;

import hittable;
import interval;
import aabb;

/**
 * @class hittable_list
 * @brief Represents a list of hittable objects
 */
export class hittable_list : public hittable {
    public:
        std::vector<std::shared_ptr<hittable>> objects;
        /**
         * @brief Construct a new hittable list object
         */
        hittable_list() = default;
        
        /**
         * @brief Construct a new hittable list object
         * 
         * @param object The object to add to the list
         */
        explicit hittable_list(std::shared_ptr<hittable> object) {
            add(std::move(object));
        }

        /**
         * @brief Clears the list of all of its stored objects
         */
        void clear() noexcept {
            objects.clear();
        }

        /**
         * @brief Adds an hittable object to the list
         * 
         * @param object The object to add to the list
         */
        void add(std::shared_ptr<hittable> object) {
            bbox = aabb(bbox, object->bounding_box());
            objects.push_back(std::move(object));
        }

        /**
         * @brief Returns if a ray hit one the objects in the list
         * 
         * @param r The ray vector
         * @param ray_t The valid interval [t_min, t_max] along the ray's trajectory where intersections are allowed to register
         * @param rec The hit record used to store hit information if a hit occured
         * @return true if the ray vector hit at least one object in the 3D space
         * @return false if the ray did not hit at least one object in the 3D space
         */
        [[nodiscard]]
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec{};
            bool hit_anything = false;
            auto closest_so_far = ray_t.max;

            for(const auto& object : objects) {
                if(object->hit(r, interval{ray_t.min, closest_so_far}, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }

        /**
         * @brief Returns the bounding box containing every hittable object from the list
         * 
         * @return The bounding box containing all hittable objects from the list
         */
        constexpr aabb bounding_box() const noexcept override {
            return bbox;
        }

    private:
        aabb bbox;
};