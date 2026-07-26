/**
 * @file aabb.cppm
 * @brief A class representing a bounding box
*/

export module aabb;

import vec3;
import ray;
import interval;

/**
 * @class aabb
 * @brief A class representing a bounding box in a 3D space
 */
export class aabb {
    public:
        interval x, y, z;

        /**
         * @brief Constructs a default aabb box
         */
        constexpr aabb() noexcept = default;

        /**
         * @brief Constructs an aabb box with the specified intervals
         * 
         * @param x The interval on the X axis
         * @param y The interval on the Y axis
         * @param z The interval on the Z axis
         */
        constexpr aabb(const interval& x, const interval& y, const interval& z) noexcept : x{x}, y{y}, z{z} {}

        /**
         * @brief Constructs an aabb box using two points
         * 
         * @param a The first point defining the bounding box
         * @param b The second point defining the bounding box
         */
        constexpr aabb(const point3& a, const point3& b) noexcept {
            x = (a[0] <= b[0] ? interval{a[0], b[0]} : interval{b[0], a[0]});
            y = (a[1] <= b[1] ? interval{a[1], b[1]} : interval{b[1], a[1]});
            z = (a[2] <= b[2] ? interval{a[2], b[2]} : interval{b[2], a[2]});
        }

        /**
         * @brief Constructs an aabb bounding box from two different bounding boxes by enclosing each interval
         * 
         * @param box0 The first box
         * @param box1 The second box
         */
        constexpr aabb(const aabb& box0, const aabb& box1) noexcept {
            x = interval{box0.x, box1.x};
            y = interval{box0.y, box1.y};
            z = interval{box0.z, box1.z};
        }

        /**
         * @brief Returns the specified axis of the bounding box
                  0 for the X axis
                  1 for the Y axis
                  2 for the Z axis
         * 
         * @param n The axis to return
         * @return The interval corresponding to the specified axis of the bounding box
         */
        constexpr const interval& axis_interval(int n) const noexcept {
            if (n == 1) return y;
            if (n == 2) return z;
            return x;
        }

        /**
         * @brief Returns if a ray hit the bounding box
         * 
         * @param r The incoming ray
         * @param ray_t The valid interval [t_min, t_max] along the ray's trajectory where intersections are allowed to register.
         * @return true if the incoming ray hit the bounding box
         * @return false if the incoming ray did not hit the bounding box
         */
        [[nodiscard]]
        constexpr bool hit(const ray& r, interval ray_t) const {
            const point3& ray_orig = r.origin();
            const vec3& ray_dir = r.direction();

            for(int axis = 0; axis < 3; axis++) {
                const interval& ax = axis_interval(axis);
                const double adinv = 1.0 / ray_dir[axis];

                auto t0 = (ax.min - ray_orig[axis]) * adinv;
                auto t1 = (ax.max - ray_orig[axis]) * adinv;

                if(t0 < t1) {
                    if (t0 > ray_t.min) ray_t.min = t0;
                    if (t1 < ray_t.max) ray_t.max = t1;
                } else {
                    if (t1 > ray_t.min) ray_t.min = t1;
                    if (t0 < ray_t.max) ray_t.max = t0;
                }

                if(ray_t.max <= ray_t.min) return false;
            }
            return true;
        }

        /**
         * @brief Returns the index of the longest axis of the bounding box
         * 
         * @return The index of the longest axis of the bounding box
         */
        [[nodiscard]]
        constexpr int longest_axis() const {
            if (x.size() > y.size())
                return x.size() > z.size() ? 0 : 2;
            else
                return y.size() > z.size() ? 1 : 2;
        }

        static const aabb empty, universe;
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);