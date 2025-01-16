/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Point.hpp"
#include "Vector.hpp"

namespace RayTracer {
    class Rectangle {
    public:
        Math::Point<3> origin;
        Math::Vector<3> bottom_side;
        Math::Vector<3> left_side;

        Rectangle() = default;

        ~Rectangle() = default;

        Rectangle(const Math::Point<3> &o, const Math::Vector<3> &b, const Math::Vector<3> &l)
                : origin(o), bottom_side(b), left_side(l) {};

        Rectangle(const Rectangle& other) = default;

        [[nodiscard]] Math::Point<3> pointAt(double u, double v) const {
            return Math::Point<3>(origin + (bottom_side * u) + (left_side * v));
        }
    };
}

#endif
