
module;

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../external/stb_image.h"
#include <string>
#include <print>
#include <iostream>

export module image;

export class image {
public:
    image() = default;

    /**
     * @brief Construct a new image object by searching the texture file in the "images" folder
     * 
     * @param image_filename The filename of the texture
     */
    explicit image(const char * image_filename) {
        auto filename = std::string(image_filename);
        auto * imagedir = std::getenv("RTW_IMAGES");

        if(imagedir && load(std::string(imagedir) + "/" + image_filename)) return;
        if(load(filename)) return;
        if(load("images/" + filename)) return;
        if(load("../images/" + filename)) return;
        if(load("../../images/" + filename)) return;
        if(load("../../../images/" + filename)) return;
        if(load("../../../../images/" + filename)) return;
        if(load("../../../../../images/" + filename)) return;
        if(load("../../../../../../images/" + filename)) return;

        std::println(std::cerr, "ERROR: Could not load image file '{}'", image_filename);
    }

    /**
     * @brief Destroy the image object
     */
    ~image() {
        delete[] bdata;
        STBI_FREE(fdata);
    }

    image(const image&) = delete;

    image& operator=(const image&) = delete;

    image(image&&) = delete;

    image& operator=(image&&) = delete;

    /**
     * @brief Load a texture from the given filename
     * 
     * @param filename The name of the texture file to load
     * @return true If the texture file was successfully loaded
     * @return false If the texture loading went wwrong
     */
    bool load(const std::string& filename) {
        auto n = bytes_per_pixel;

        fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);

        if(!fdata) return false;

        bytes_per_scanline = image_width * bytes_per_pixel;
        convert_to_bytes();
        return true;
    }

    /**
     * @brief Returns the width of the texture
     * 
     * @return The width of the texture or 0 if the loading failed
     */
    [[nodiscard]]
    constexpr int width() const noexcept {
        return fdata ? image_width : 0;
    }

    /**
     * @brief Returns the height of the texture
     * 
     * @return The height of the texture or 0 if the loading failed
     */
    [[nodiscard]]
    constexpr int height() const noexcept {
        return fdata ? image_height : 0;
    }

    /**
     * @brief Returns the address of the three RGB bytes of the pixel at x,y. If there is no image data, returns magenta.
     * 
     * @param x The first coordinate of the pixel
     * @param y The second coordinate of the pixel
     * @return The address of the RGB bytes of the pixel at the desired coordinates
     */
    [[nodiscard]]
    constexpr const unsigned char * pixel_data(int x, int y) const noexcept {
        static constexpr unsigned char magenta[] = {255, 0, 255};

        if (!bdata) return magenta;

        x = clamp(x, 0, image_width);
        y = clamp(y, 0, image_height);

        return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
    }

private:
    static constexpr int bytes_per_pixel = 3;

    float * fdata = nullptr;
    unsigned char * bdata = nullptr;

    int image_width = 0;
    int image_height = 0;
    int bytes_per_scanline = 0;

    /**
     * @brief Returns the value clamped to the range [low, high)
     * 
     * @param x The value to clamp
     * @param low The lower bound to clamp the value
     * @param high The upper bound to clamp the value
     * @return The value clamped to the range [low, high)
     */
    static int clamp(int x, int low, int high) {
        if(x < low) 
            return low;
        if(x < high) 
            return x;
        return high - 1;
    }

    /**
     * @brief Converts the given float to byte
     * 
     * @param value The float value to convert
     * @return The corresponding value in byte
     */
    [[nodiscard]]
    static unsigned char float_to_byte(float value) {
        if(value <= 0.0)
            return 0;
        if(value >= 1.0)
            return 255;

        return static_cast<unsigned char>(value * 256.0);
    }

    /**
     * @brief Converts the linear floating point pixel data to bytes, storing the resulting byte data in the "bdata" member.
     */
    void convert_to_bytes() {
        const int total_bytes = image_width * image_height * bytes_per_pixel;

        delete[] bdata;
        bdata = new unsigned char[total_bytes];

        auto* bptr = bdata;
        auto* fptr = fdata;

        for(int i = 0; i < total_bytes; ++i)
            *bptr++ = float_to_byte(*fptr++);
    }
};