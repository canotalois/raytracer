/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef GEOMETRY_BASE_HPP
#define GEOMETRY_BASE_HPP

#include "Matrix.hpp"

namespace Math {
    template<std::size_t N, typename T = double>
    class GeometryBase : public Matrix<N, 1, T> {
    public:
        using Matrix<N, 1, T>::Matrix;

        explicit GeometryBase(const Matrix<N, 1, T>& mat) : Matrix<N, 1, T>(mat) {}

        // Common operators
        GeometryBase& operator+=(const GeometryBase& rhs) {
            for (size_t i = 0; i < N; ++i) {
                this->data[i] += rhs.data[i];
            }
            return *this;
        }

        GeometryBase& operator-=(const GeometryBase& rhs) {
            for (size_t i = 0; i < N; ++i) {
                this->data[i] -= rhs.data[i];
            }
            return *this;
        }

        T dot(const GeometryBase<N, T>& other) const {
            T result = 0;
            for (size_t i = 0; i < N; ++i) {
                result += this->data[i] * other.data[i];
            }
            return result;
        }
    };

    template<std::size_t N, typename T>
    GeometryBase<N, T> operator+(const GeometryBase<N, T>& lhs, const GeometryBase<N, T>& rhs) {
        GeometryBase<N, T> result = lhs;
        result += rhs;
        return result;
    }

    template<std::size_t N, typename T>
    GeometryBase<N, T> operator-(const GeometryBase<N, T>& lhs, const GeometryBase<N, T>& rhs) {
        GeometryBase<N, T> result = lhs;
        result -= rhs;
        return result;
    }
}

#endif
