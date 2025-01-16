/*
 * ᗧ···ᗣ···ᗣ··
*/

#include "Point.hpp"

namespace Math {

    // Addition assignment
    template<std::size_t N, typename T>
    Point<N, T>& Point<N, T>::operator+=(const Point<N, T>& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            this->data[i] += rhs.data[i];
        }
        return *this;
    }

    // Subtraction assignment
    template<std::size_t N, typename T>
    Point<N, T>& Point<N, T>::operator-=(const Point<N, T>& rhs) {
        for (std::size_t i = 0; i < N; ++i) {
            this->data[i] -= rhs.data[i];
        }
        return *this;
    }

    // Addition
    template<std::size_t N, typename T>
    Point<N, T> Point<N, T>::operator+(const Point<N, T>& other) const {
        Point<N, T> result;
        for (std::size_t i = 0; i < N; ++i) {
            result.data[i] = this->data[i] + other.data[i];
        }
        return result;
    }

    // Subtraction
    template<std::size_t N, typename T>
    Point<N, T> Point<N, T>::operator-(const Point<N, T>& other) const {
        Point<N, T> result;
        for (std::size_t i = 0; i < N; ++i) {
            result.data[i] = this->data[i] - other.data[i];
        }
        return result;
    }

    // Scalar multiplication
    template<std::size_t N, typename T>
    Point<N, T> Point<N, T>::operator*(T scalar) const {
        Point<N, T> result;
        for (std::size_t i = 0; i < N; ++i) {
            result.data[i] = this->data[i] * scalar;
        }
        return result;
    }

    // Scalar division
    template<std::size_t N, typename T>
    Point<N, T> Point<N, T>::operator/(T scalar) const {
        if (scalar == T(0)) {
            throw std::runtime_error("Division by zero.");
        }
        Point<N, T> result;
        for (std::size_t i = 0; i < N; ++i) {
            result.data[i] = this->data[i] / scalar;
        }
        return result;
    }

    // Explicit template instantiation for common types
    template class Point<2, double>;
    template class Point<3, double>;
    template class Point<4, double>;
}
