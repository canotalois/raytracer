/*
 * ᗧ···ᗣ···ᗣ··
*/
#ifndef SAFE_BUFFER_HPP
#define SAFE_BUFFER_HPP
#include <cstdlib>
#include "Color.hpp"
class SafeBuffer {
public:
    SafeBuffer(int width, int height) : width(width), height(height) {
        pixels = new uint8_t[width * height * 4]();
    }
    ~SafeBuffer() {
        delete[] pixels;
    }
    void resize(int newWidth, int newHeight) {
        std::lock_guard<std::mutex> lock(mutex);
        delete[] pixels;
        width = newWidth;
        height = newHeight;
        pixels = new uint8_t[width * height * 4]();
    }
    void setBlockColor(int startX, int startY, int blockWidth, int blockHeight, const RayTracer::Color& color) {
        std::lock_guard<std::mutex> lock(mutex);
        auto byte_color = color.to_byte_color();
        auto red = byte_color[0];
        auto green = byte_color[1];
        auto blue = byte_color[2];
        for (int y = startY; y < startY + blockHeight && y < height; ++y) {
            for (int x = startX; x < startX + blockWidth && x < width; ++x) {
                int index = (y * width + x) * 4;
                pixels[index] = red;
                pixels[index + 1] = green;
                pixels[index + 2] = blue;
                pixels[index + 3] = 255;
            }
        }
    }
    void setPixelColor(int x, int y, const RayTracer::Color& color) const {
        std::lock_guard<std::mutex> lock(mutex);
        auto byte_color = color.to_byte_color();
        auto red = byte_color[0];
        auto green = byte_color[1];
        auto blue = byte_color[2];
        int index = (y * width + x) * 4;
        pixels[index] = red;
        pixels[index + 1] = green;
        pixels[index + 2] = blue;
        pixels[index + 3] = 255;
    }
    uint8_t* getData() {
        std::lock_guard<std::mutex> lock(mutex);
        return pixels;
    }
private:
    uint8_t* pixels;
    mutable std::mutex mutex;
    int width, height;
};
#endif