/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "ALight.hpp"

namespace RayTracer {
    class PointLight : public ALight {
        Math::Point<3> position;

    public:
        PointLight(const Math::Point<3> &position, const RayTracer::Color &col) : position(position),
                                                                                  ALight(col) {}

        [[nodiscard]] Math::Point<3> getLightPosition() const {
            return position;
        }

        [[nodiscard]] Math::Vector<3> getLightDirection(const Math::Point<3>& point) const override {
            return Math::Vector<3>((position - point)).normalize();
        }

        [[nodiscard]] double getAttenuation(double distance, const PhongReflectionModel& phongModel) const override {
            return 1.0 / (phongModel.constantAttenuation + phongModel.linearAttenuation * distance + phongModel.quadraticAttenuation * distance * distance);
        }
        [[nodiscard]] bool castsShadows() const override {
            return true;
        }
    };
}

/*

namespace RayTracer {
    class PointLight : public ALight {
        Math::Point<3> position;

    public:
        PointLight(const Math::Point<3> &position, const RayTracer::Color &col) : position(position),
                                                                                  ALight(col) {}

        [[nodiscard]] Math::Point<3> getLightPosition() const {
            return position;
        }
    };
}

 */

#endif
