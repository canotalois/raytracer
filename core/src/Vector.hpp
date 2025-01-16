/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "GeometryBase.hpp"
#include <cmath>
#include <stdexcept>

namespace Math {
    template<std::size_t N, typename T = double>
    class Vector : public GeometryBase<N, T> {
    public:
        using GeometryBase<N, T>::GeometryBase;

        explicit Vector(const Matrix<N, 1, T>& mat) : GeometryBase<N, T>(mat) {}

        Vector() = default;
        ~Vector() = default;
        Vector(const Vector& other) = default;
        Vector<N, T>& operator=(const Vector<N, T>& other) = default;

        [[nodiscard]] double length() const;
        Vector normalize() const;
        static Vector<3, T> random_in_unit_sphere();
        static Vector<3, T> random_unit_vector();
        static Vector<3, T> random_on_hemisphere(const Vector<3, T>& normal);
        [[nodiscard]] bool near_zero() const;
        static Vector<3, T> reflect(const Vector<3, T>& v, const Vector<3, T>& n);
        static Vector<3, T> refract(const Vector<3, T>& uv, const Vector<3, T>& n, double etai_over_etat);
        static Vector<3, T> cross(const Vector<3, T>& u, const Vector<3, T>& v);

        Vector<N, T>& operator+=(const Vector<N, T>& rhs);
        Vector<N, T>& operator-=(const Vector<N, T>& rhs);
        Vector<N, T> operator+(const Vector<N, T>& other) const;
        Vector<N, T> operator-(const Vector<N, T>& other) const;
        Vector<N, T> operator*(T scalar) const;
        Vector<N, T> operator/(T scalar) const;
        Vector<N, T> operator-() const;
    };
}

#endif
