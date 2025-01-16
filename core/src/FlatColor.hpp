/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef FLAT_COLOR_HPP
#define FLAT_COLOR_HPP

#include "AMaterial.hpp"

namespace RayTracer {
    class FlatColor : public AMaterial {
    public:
        Color color;

        explicit FlatColor(const Color& c) : color(c) {}

        bool scatter(
                const Ray& r_in, const Intersection& rec, Color& attenuation, Ray& scattered
        ) const override {
            attenuation = color;
            return false;
        }

        [[nodiscard]] bool usePhongModel() const override {
            return true;
        }
    };
}

#endif
