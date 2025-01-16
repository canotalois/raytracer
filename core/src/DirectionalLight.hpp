/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "ALight.hpp"

namespace RayTracer {
    class DirectionalLight : public ALight {
        Math::Vector<3> direction;

    public:
        DirectionalLight(const Math::Vector<3> &dir, const RayTracer::Color &col) : direction(dir.normalize()),
                                                                                    ALight(col) {}

        [[nodiscard]] Math::Vector<3> getLightDirection(const Math::Point<3>&) const override {
            return direction;
        }

        [[nodiscard]] bool castsShadows() const override {
            return true;
        }
    };
}

#endif
