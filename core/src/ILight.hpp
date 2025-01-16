/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef ILIGHT_HPP
#define ILIGHT_HPP

#include "Color.hpp"
#include "Vector.hpp"
#include "Point.hpp"
#include "PhongReflectionModel.hpp"

namespace RayTracer {
    class ILight {
    public:
        virtual ~ILight() = default;
        [[nodiscard]] virtual RayTracer::Color getLightColor() const = 0;
        [[nodiscard]] virtual Math::Vector<3> getLightDirection(const Math::Point<3>& point) const = 0;
        [[nodiscard]] virtual double getAttenuation(double distance, const PhongReflectionModel& phongModel) const = 0;
        [[nodiscard]] virtual bool castsShadows() const { return true; }
    };
}

/*

namespace RayTracer {
    class ILight {
    public:
        virtual ~ILight() = default;
        [[nodiscard]] virtual RayTracer::Color getLightColor() const = 0;
    };
}

 */

#endif

/*
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

    Math::Point<3> getLightPosition() const override {
        return Math::Point < 3 > {0, 0, 0};
    }

    bool isWithinCone(const Math::Point<3>& point) const {
        Math::Vector<3> toPoint = Math::Vector<3>(point - position).normalize();
        return direction.dot(toPoint) > cosTheta;
    }
};

*/