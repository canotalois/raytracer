/*
 * ᗧ···ᗣ···ᗣ··
*/

#include "Vector.hpp"

namespace Math {

    template<std::size_t N, typename T>
    double Vector<N, T>::length() const {
        double sum = 0;
        for (std::size_t i = 0; i < N; ++i) {
            sum += this->data[i] * this->data[i];
        }
        return std::sqrt(sum);
    }

    template<std::size_t N, typename T>
    Vector<N, T> Vector<N, T>::normalize() const {
        double len = length();
        if (len == 0) return Vector<N>(); // throw std::runtime_error("Attempt to normalize a zero-length vector");
        Vector result;
        for (std::size_t i = 0; i < N; ++i) {
            result.data[i] = this->data[i] / len;
        }
        return result;
    }

    template<std::size_t N, typename T>
    bool Vector<N, T>::near_zero() const {
        const auto s = 1e-8;
        for (std::size_t i = 0; i < N; ++i) {
            if (std::fabs(this->data[i]) >= s) return false;
        }
        return true;
    }

    // Assuming these methods are specifically for 3D vectors

    template<std::size_t N, typename T>
    Vector<3, T> Vector<N, T>::random_in_unit_sphere() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<T> dis(0.0, 1.0);

        while (true) {
            Vector<3, T> p({dis(gen), dis(gen), dis(gen)});
            if (p.length() >= 1) continue;
            return p;
        }
    }

    template<std::size_t N, typename T>
    Vector<3, T> Vector<N, T>::random_unit_vector() {
        return random_in_unit_sphere().normalize();
    }

    template<std::size_t N, typename T>
    Vector<3, T> Vector<N, T>::random_on_hemisphere(const Vector<3, T>& normal) {
        Vector<3, T> on_unit_sphere = random_unit_vector();
        if (on_unit_sphere.dot(normal) > 0.0)  // Same hemisphere as the normal
            return on_unit_sphere;
        else
            return -on_unit_sphere;  // Reverse it
    }

    template<std::size_t N, typename T>
    Vector<3, T> Vector<N, T>::reflect(const Vector<3, T>& v, const Vector<3, T>& n) {
        return v - n * 2 * v.dot(n);
    }

    template<std::size_t N, typename T>
    Vector<3, T> Vector<N, T>::refract(const Vector<3, T>& uv, const Vector<3, T>& n, double etai_over_etat) {
        auto cos_theta = fmin((-uv).dot(n), 1.0);
        Vector<3, T> r_out_perp =  (uv + n * cos_theta) * etai_over_etat;
        Vector<3, T> r_out_parallel = n * -sqrt(fabs(1.0 - r_out_perp.length() * r_out_perp.length()));
        return r_out_perp + r_out_parallel;
    }

    template<std::size_t N, typename T>
    Vector<3, T> Vector<N, T>::cross(const Vector<3, T> &u, const Vector<3, T> &v) {
        return Vector<3, T>{
                u.data[1] * v.data[2] - u.data[2] * v.data[1],  // x component
                u.data[2] * v.data[0] - u.data[0] * v.data[2],  // y component
                u.data[0] * v.data[1] - u.data[1] * v.data[0]   // z component
        };
    }

    // Arithmetic Operators

    template<std::size_t N, typename T>
    Vector<N, T>& Vector<N, T>::operator+=(const Vector<N, T>& rhs) {
        for (size_t i = 0; i < N; ++i) {
            this->data[i] += rhs.data[i];
        }
        return *this;
    }

    template<std::size_t N, typename T>
    Vector<N, T>& Vector<N, T>::operator-=(const Vector<N, T>& rhs) {
        for (size_t i = 0; i < N; ++i) {
            this->data[i] -= rhs.data[i];
        }
        return *this;
    }

    template<std::size_t N, typename T>
    Vector<N, T> Vector<N, T>::operator+(const Vector<N, T>& other) const {
        Vector result;
        for (size_t i = 0; i < N; ++i) {
            result.data[i] = this->data[i] + other.data[i];
        }
        return result;
    }

    template<std::size_t N, typename T>
    Vector<N, T> Vector<N, T>::operator-(const Vector<N, T>& other) const {
        Vector result;
        for (size_t i = 0; i < N; ++i) {
            result.data[i] = this->data[i] - other.data[i];
        }
        return result;
    }

    template<std::size_t N, typename T>
    Vector<N, T> Vector<N, T>::operator*(T scalar) const {
        Vector result;
        for (size_t i = 0; i < N; ++i) {
            result.data[i] = this->data[i] * scalar;
        }
        return result;
    }

    template<std::size_t N, typename T>
    Vector<N, T> Vector<N, T>::operator/(T scalar) const {
        if (scalar == T(0)) throw std::runtime_error("Division by zero.");
        Vector result;
        for (size_t i = 0; i < N; ++i) {
            result.data[i] = this->data[i] / scalar;
        }
        return result;
    }

    // Negation operator
    template<std::size_t N, typename T>
    Vector<N, T> Vector<N, T>::operator-() const {
        Vector<N, T> result;
        for (size_t i = 0; i < N; ++i) {
            result.data[i] = -this->data[i];
        }
        return result;
    }

    // Explicit template instantiation
    template class Vector<2, double>;
    template class Vector<3, double>;
    template class Vector<4, double>;
}
