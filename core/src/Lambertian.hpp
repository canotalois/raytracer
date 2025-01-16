/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "AMaterial.hpp"

namespace RayTracer {
    class Lambertian : public AMaterial {
    public:
        Color albedo;

        explicit Lambertian(const Color &a) : albedo(a) {}

        bool scatter(
                const Ray &r_in, const Intersection &rec, Color &attenuation, Ray &scattered
        ) const override {
            auto scatter_direction = rec.normal + Math::Vector<3>::random_unit_vector();
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            scattered = Ray(rec.intersection, scatter_direction);
            attenuation = albedo;
            return true;
        }

        [[nodiscard]] bool usePhongModel() const override {
            return false;
        }
    };
}

#endif
