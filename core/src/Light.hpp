/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Color.hpp"
#include "Vector.hpp"
#include "Point.hpp"

namespace RayTracer {
    class Light {
    public:
        virtual RayTracer::Color getLightColor() const = 0;

        virtual Math::Vector<3> getLightDirection(const Math::Point<3> &point) const = 0;
    };

    class DirectionalLight : public Light {
        Math::Vector<3> direction;
        RayTracer::Color color;

    public:
        DirectionalLight(const Math::Vector<3> &dir, const RayTracer::Color &col) : direction(dir.normalize()),
                                                                                    color(col) {}

        RayTracer::Color getLightColor() const override {
            return color;
        }

        Math::Vector<3> getLightDirection(const Math::Point<3> &) const override {
            return direction;
        }
    };

    class AmbientLight : public Light {
        RayTracer::Color color;

    public:
        AmbientLight(const RayTracer::Color &col) : color(col) {}

        RayTracer::Color getLightColor() const override {
            return color;
        }

        Math::Vector<3> getLightDirection(const Math::Point<3> &) const override {
            return Math::Vector < 3 > {0, 0, 0};
        }
    };

    class SpotLight : public Light {
        Math::Point<3> position;
        Math::Vector<3> direction; // Calculé comme la normalisation de (target - position)
        RayTracer::Color color;
        double angle;  // Angle du cône de lumière, généralement en degrés
        double cosTheta;  // Cosinus de l'angle, précalculé pour une utilisation dans les calculs d'éclairage

    public:
        SpotLight(const Math::Point<3>& pos, const Math::Point<3>& target, const RayTracer::Color& col, double ang)
                : position(pos), direction(Math::Vector<3>(target - pos).normalize()), color(col), angle(ang) {
            cosTheta = cos(angle * M_PI / 180.0);
        }

        RayTracer::Color getLightColor() const override {
            return color;
        }

        Math::Vector<3> getLightDirection(const Math::Point<3>& point) const override {
            return direction;
        }

        bool isWithinCone(const Math::Point<3>& point) const {
            Math::Vector<3> toPoint = Math::Vector<3>(point - position).normalize();
            return direction.dot(toPoint) > cosTheta;
        }
    };

}

#endif