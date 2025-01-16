/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <iostream>
#include "Interval.hpp"

namespace RayTracer {
    class Color {
    private:
        std::array<double, 3> rgb;
    public:
        Color() : rgb({0.0, 0.0, 0.0}) {}

        Color(double r, double g, double b) : rgb({r, g, b}) {}

        [[nodiscard]] double r() const { return rgb[0]; }
        [[nodiscard]] double g() const { return rgb[1]; }
        [[nodiscard]] double b() const { return rgb[2]; }

        Color operator*(double scalar) const {
            return {rgb[0] * scalar, rgb[1] * scalar, rgb[2] * scalar};
        }

        Color& operator*=(double scalar) {
            rgb[0] *= scalar;
            rgb[1] *= scalar;
            rgb[2] *= scalar;
            return *this;
        }

        Color operator*(const Color& other) const {
            return {rgb[0] * other.rgb[0], rgb[1] * other.rgb[1], rgb[2] * other.rgb[2]};
        }

        Color operator+(const Color& other) const {
            return {rgb[0] + other.rgb[0], rgb[1] + other.rgb[1], rgb[2] + other.rgb[2]};
        }

        Color& operator+=(const Color& other) {
            rgb[0] += other.rgb[0];
            rgb[1] += other.rgb[1];
            rgb[2] += other.rgb[2];
            return *this;
        }

        [[nodiscard]] std::array<uint8_t, 3> to_byte_color() const {
            static const Interval intensity(0.000, 0.999);

            auto clamped_r = intensity.clamp(linear_to_gamma(r()));
            auto clamped_g = intensity.clamp(linear_to_gamma(g()));
            auto clamped_b = intensity.clamp(linear_to_gamma(b()));

            auto red = static_cast<uint8_t>(clamped_r * 256);
            auto green = static_cast<uint8_t>(clamped_g * 256);
            auto blue = static_cast<uint8_t>(clamped_b * 256);

            return {red, green, blue};
        }

        friend std::ostream& operator<<(std::ostream& os, const Color& c) {
            os << static_cast<int>(c.rgb[0] * 255.999) << " "
               << static_cast<int>(c.rgb[1] * 255.999) << " "
               << static_cast<int>(c.rgb[2] * 255.999);
            return os;
        }
    };
}

#endif
