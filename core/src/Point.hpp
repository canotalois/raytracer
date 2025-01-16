/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef POINT_HPP
#define POINT_HPP

#include "GeometryBase.hpp"

namespace Math {
    template<std::size_t N, typename T = double>
    class Point : public GeometryBase<N, T> {
    public:
        using GeometryBase<N, T>::GeometryBase;
        using BaseType = Matrix<N, 1, T>;

        explicit Point(const BaseType& mat) : GeometryBase<N, T>(mat) {}

        Point() = default;
        ~Point() = default;
        Point(const Point& other) = default;
        Point<N, T>& operator=(const Point<N, T>& other) = default;

        Point<N, T>& operator+=(const Point<N, T>& rhs);
        Point<N, T>& operator-=(const Point<N, T>& rhs);
        Point<N, T> operator+(const Point<N, T>& other) const;
        Point<N, T> operator-(const Point<N, T>& other) const;
        Point<N, T> operator*(T scalar) const;
        Point<N, T> operator/(T scalar) const;
    };
}

#endif
