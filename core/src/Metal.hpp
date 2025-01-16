/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef METAL_HPP
#define METAL_HPP

#include "AMaterial.hpp"

namespace RayTracer {
    class Metal : public AMaterial {
    public:
        Color albedo;
        double fuzz;

        Metal(const Color &a, double f) : albedo(a), fuzz(f) {}

        bool scatter(
                const Ray &r_in, const Intersection &rec, Color &attenuation, Ray &scattered
        ) const override {
            auto reflected = Math::Vector<3>::reflect(r_in.direction.normalize(), rec.normal);
            scattered = Ray(rec.intersection, (Math::Vector<3>::random_in_unit_sphere() * fuzz + reflected));
            attenuation = albedo;
            return (scattered.direction.dot(rec.normal) > 0);
        }
    };
}

#endif
