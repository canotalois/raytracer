/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef ALIGHT_HPP
#define ALIGHT_HPP

#include "ILight.hpp"

namespace RayTracer {
    class ALight : public ILight {
    public:
        explicit ALight(const RayTracer::Color &color): color(color) {};
        ~ALight() override = 0;

        [[nodiscard]] RayTracer::Color getLightColor() const override {
            return this->color;
        };

        [[nodiscard]] Math::Vector<3> getLightDirection(const Math::Point<3>&) const override {
            return Math::Vector<3>{0.0, 0.0, 0.0};
        }

        [[nodiscard]] double getAttenuation(double, const PhongReflectionModel&) const override {
            return 1.0; // Pas d'atténuation par defaut
        }

        [[nodiscard]] bool castsShadows() const override {
            return false;
        }

    private:
        RayTracer::Color color;
    };
}

#endif
