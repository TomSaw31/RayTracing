/**
 * @file texture.cppm
 * @brief A texture class used for render
*/
module;

#include <memory>
#include <cmath>

export module texture;

import vec3;
import color;
import image;
import interval;
import perlin;

/**
 * @class texture
 * @brief Represents a texture used on a geometry object for render
 */
export class texture {
    public:
        virtual ~texture() = default;

        /**
         * @brief Retursn the color value at the texture coordinates u,v
         * 
         * @param u The first texture coordinate
         * @param v The second texture coordinate
         * @param p The point in the 3D space corresponding to the texture coordinates
         * @return The color value at the corresponding coordinates
         */
        [[nodiscard]]
        constexpr virtual color value(double u, double v, const point3& p) const noexcept = 0;
};

/**
 * @class solid_color
 * @brief A solid texture with a simple and unique color
 */
export class solid_color : public texture {
    public:
        /**
         * @brief Constructs a solid color texture from the given color
         *
         * @param albedo The color of the solid color texture
         */
        constexpr solid_color(const color& albedo) noexcept : albedo{albedo} {}

        /**
         * @brief Constructs a solid color texture from the RGB values
         * 
         * @param red The red channel value
         * @param green The green channel value
         * @param blue The blue channel value
         */
        constexpr solid_color(double red, double green, double blue) noexcept : solid_color{color{red, green, blue}} {}

        /**
         * @brief Returns the color of the texture
         * 
         * @param u unused
         * @param v unused
         * @param p unused
         * @return The color of the texture
         */
        [[nodiscard]]
        constexpr color value([[maybe_unused]] double u, [[maybe_unused]] double v, [[maybe_unused]] const point3& p) const noexcept override {
            return albedo;
        }

    private:
        color albedo;
};

/**
 * @class checker_texture
 * @brief A class representing a checker pattern texture
 * 
 */
export class checker_texture : public texture {
    public:
        /**
         * @brief Constructs a checker texture
         * 
         * @param scale The scale of the checker pattern*
         * @param even The texture to display on the even tiles
         * @param odd The texture to display on the odd tiles
         */
        constexpr checker_texture(double scale, std::shared_ptr<texture> even, std::shared_ptr<texture> odd) 
            : inv_scale{1.0 / scale}, even{even}, odd{odd} {}

        /**
         * @brief Constructs a checker texture
         * 
         * @param scale The scale of the checker pattern*
         * @param c1 The color to display on the even tiles
         * @param c2 The color to display on the odd tiles
         */
        constexpr checker_texture(double scale, const color& c1, const color& c2)
            : checker_texture{scale, std::make_shared<solid_color>(c1), std::make_shared<solid_color>(c2)} {}

        /**
         * @brief Returns the color value at the texture coordinates u,v on the checker
         * 
         * @param u The first texture coordinate
         * @param v The second texture coordinate
         * @param p The point in the 3D space corresponding to the texture coordinates
         * @return The color value at the corresponding coordinates
         */
        [[nodiscard]]
        constexpr color value(double u, double v, const point3& p) const noexcept override {
            auto xInteger = static_cast<int>(floor(inv_scale * p.x()));
            auto yInteger = static_cast<int>(floor(inv_scale * p.y()));
            auto zInteger = static_cast<int>(floor(inv_scale * p.z()));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        double inv_scale{1.0};
        std::shared_ptr<texture> even{nullptr};
        std::shared_ptr<texture> odd{nullptr};
};

/**
 * @class image_texture
 * @brief A class representing an image loaded from a filename on a texture
 */
export class image_texture : public texture {
    public:
        /**
         * @brief Constructs an image texture from a filename
         * 
         * @param filenamee The filename of the image
         */
        constexpr image_texture(const char * filename) noexcept : image{filename} {}

        /**
         * @brief Returns the color value at the texture coordinates u,v on the image
         * 
         * @param u The first texture coordinate
         * @param v The second texture coordinate
         * @param p unused
         * @return The color value at the corresponding coordinates
         */
        [[nodiscard]]
        constexpr color value(double u, double v, [[maybe_unused]] const point3& p) const noexcept override {
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

/**
 * @class noise_texture
 * @brief A class used to generates random texutres containing a perlin noise pattern
 */
export class noise_texture : public texture {
    public:
        /**
         * @brief Construct a new noise texture
         * 
         * @param scale The scale factor for the noise
         */
        noise_texture(double scale) : scale{scale} {}

        /**
         * @brief Returns the value on the perlin noise texture
         * 
         * @param u The first texture coordinate
         * @param v The second texture coordinate
         * @param p The corresponding point in the 3D space
         * @return The color value at the corresponding coordinates
         */
        [[nodiscard]]
        constexpr color value(double u, double v, const point3& p) const noexcept override {
            return color(0.5, 0.5, 0.5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
        }

    private:
        perlin noise;
        double scale{1.0};
};