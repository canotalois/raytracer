/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef IMATERIAL_HPP
#define IMATERIAL_HPP

#include "Ray.hpp"
#include "Color.hpp"
#include "Intersection.hpp"

namespace RayTracer {
    class IMaterial {
    public:
        virtual ~IMaterial() = default;

        virtual bool scatter(const Ray& r_in, const Intersection& rec, Color& attenuation, Ray& scattered) const = 0;

        virtual bool usePhongModel() const = 0;
    };
}

#endif
