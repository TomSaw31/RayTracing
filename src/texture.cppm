/**
 * @file texture.cppm
 * @brief A texture class used for render
*/
module;

#include <memory>

export module texture;

import vec3;
import color;
import image;
import interval;

export class texture {
    public:
        virtual ~texture() = default;

        [[nodiscard]]
        constexpr virtual color value(double u, double v, const point3& p) const noexcept = 0;
};

export class solid_color : public texture {
    public:
        constexpr solid_color(const color& albedo) noexcept : albedo{albedo} {}

        constexpr solid_color(double red, double green, double blue) noexcept : solid_color{color{red, green, blue}} {}

        [[nodiscard]]
        constexpr color value(double u, double v, const point3& p) const noexcept override {
            return albedo;
        }

    private:
        color albedo;
};

export class checker_texture : public texture {
    public:
        constexpr checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd) 
            : inv_scale{1.0 / scale}, even{even}, odd{odd} {}

        constexpr checker_texture(double scale, const color& c1, const color& c2)
            : checker_texture{scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)} {}

        [[nodiscard]]
        constexpr color value(double u, double v, const point3& p) const noexcept override {
            auto xInteger = static_cast<int>(floor(inv_scale * p.x()));
            auto yInteger = static_cast<int>(floor(inv_scale * p.y()));
            auto zInteger = static_cast<int>(floor(inv_scale * p.z()));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        double inv_scale;
        std::shared_ptr<texture> even;
        std::shared_ptr<texture> odd;
};

export class image_texture : public texture {
    public:
        constexpr image_texture(const char * filename) noexcept : image{filename} {}

        [[nodiscard]]
        constexpr color value(double u, double v, const point3& p) const noexcept override {
            if (image.height() <= 0)
                return color(0,1,1);

            u = interval(0,1).clamp(u);
            v = 1.0 - interval(0,1).clamp(v);

            auto i = static_cast<int>(u * image.width());
            auto j = static_cast<int>(v * image.height());
            auto pixel = image.pixel_data(i,j);

            auto color_scale = 1.0 / 255.0;
            return color{color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
        }
    
    private:
        image image;
};