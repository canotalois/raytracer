/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef AMBIENT_LIGHT_HPP
#define AMBIENT_LIGHT_HPP

#include "ALight.hpp"

namespace RayTracer {
    class AmbientLight : public ALight {
    public:
        explicit AmbientLight(const RayTracer::Color &col) : ALight(col) {}
    };
}

#endif
