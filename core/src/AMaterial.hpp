/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef AMATERIAL_HPP
#define AMATERIAL_HPP

#include "IMaterial.hpp"

namespace RayTracer {
    class AMaterial: public IMaterial {
    public:
        ~AMaterial() override = 0;

        bool scatter(
                const Ray& r_in, const Intersection& rec, Color& attenuation, Ray& scattered
        ) const override {
            return false;
        }

        [[nodiscard]] bool usePhongModel() const override {
            return true;
        }
    };
}

#endif
