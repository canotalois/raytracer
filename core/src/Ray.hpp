/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef RAY_HPP
#define RAY_HPP

#include "Point.hpp"
#include "Vector.hpp"
#include "Interval.hpp"
#include "Ray.hpp"

#warning need to implement get_origin ... and change value to private

namespace RayTracer {
    class Ray {
    public:
        Math::Point<3> origin;
        Math::Vector<3> direction;

        Ray() = default;
        ~Ray() = default;

        Ray(const Math::Point<3>& o, const Math::Vector<3>& d)
        : origin(o), direction(d) {}

        [[nodiscard]] Math::Point<3> at(double t) const {
            return Math::Point<3>(origin + direction * t);
        }
    };
}

#endif
