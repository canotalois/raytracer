/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <execution>
#include <algorithm>
#include <cmath>
#include <array>
#include <numeric>
#include <random>

namespace Math {

    template<std::size_t N, std::size_t M, typename T = double>
    class Matrix {
    public:
        std::array<T, N * M> data;

        // Constructors
        Matrix() : data{} {}
        Matrix(std::initializer_list<T> init);

        // Element Access
        T& at(std::size_t row, std::size_t col);
        const T& at(std::size_t row, std::size_t col) const;

        // Operators
        bool operator==(const Matrix& other) const;
        Matrix& operator+=(const Matrix& rhs);
        Matrix& operator-=(const Matrix& rhs);
        Matrix& operator*=(const T& scalar);
        Matrix& operator/=(const T& scalar);
        Matrix& operator*=(const Matrix& rhs);

        Matrix operator-() const;
        Matrix operator+(const Matrix& other) const;
        Matrix operator-(const Matrix& other) const;
        Matrix operator*(T scalar) const;
        Matrix operator/(T scalar) const;
        Matrix operator+(T scalar) const;
        Matrix operator-(T scalar) const;

        // Matrix operations
        template<std::size_t P>
        Matrix<N, P, T> operator*(const Matrix<M, P, T>& rhs) const {
            Matrix<N, P, T> result;
            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < P; ++j) {
                    T sum = 0;
                    for (std::size_t k = 0; k < M; ++k) {
                        sum += this->at(i, k) * rhs.at(k, j);
                    }
                    result.at(i, j) = sum;
                }
            }
            return result;
        }

        static Matrix<M, N, T> Transpose(const Matrix<N, M, T>& input) {
            Matrix<M, N, T> result;
            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < M; ++j) {
                    result.at(j, i) = input.at(i, j);
                }
            }
            return result;
        }

        static Matrix<N, M, T> Identity() {
            if constexpr (N == M) {
                Matrix<N, M, T> result;
                for (std::size_t i = 0; i < N; ++i) {
                    result.at(i, i) = 1;
                }
                return result;
            } else {
                throw std::logic_error("Identity matrix must be square.");
            }
        }

        T determinant() const {
            if constexpr (N == 4 && M == 4) {
                return
                        at(0, 3) * at(1, 2) * at(2, 1) * at(3, 0) - at(0, 2) * at(1, 3) * at(2, 1) * at(3, 0) -
                        at(0, 3) * at(1, 1) * at(2, 2) * at(3, 0) + at(0, 1) * at(1, 3) * at(2, 2) * at(3, 0) +
                        at(0, 2) * at(1, 1) * at(2, 3) * at(3, 0) - at(0, 1) * at(1, 2) * at(2, 3) * at(3, 0) -
                        at(0, 3) * at(1, 2) * at(2, 0) * at(3, 1) + at(0, 2) * at(1, 3) * at(2, 0) * at(3, 1) +
                        at(0, 3) * at(1, 0) * at(2, 2) * at(3, 1) - at(0, 0) * at(1, 3) * at(2, 2) * at(3, 1) -
                        at(0, 2) * at(1, 0) * at(2, 3) * at(3, 1) + at(0, 0) * at(1, 2) * at(2, 3) * at(3, 1) +
                        at(0, 3) * at(1, 1) * at(2, 0) * at(3, 2) - at(0, 1) * at(1, 3) * at(2, 0) * at(3, 2) -
                        at(0, 3) * at(1, 0) * at(2, 1) * at(3, 2) + at(0, 0) * at(1, 3) * at(2, 1) * at(3, 2) +
                        at(0, 1) * at(1, 0) * at(2, 3) * at(3, 2) - at(0, 0) * at(1, 1) * at(2, 3) * at(3, 2) -
                        at(0, 2) * at(1, 1) * at(2, 0) * at(3, 3) + at(0, 1) * at(1, 2) * at(2, 0) * at(3, 3) +
                        at(0, 2) * at(1, 0) * at(2, 1) * at(3, 3) - at(0, 0) * at(1, 2) * at(2, 1) * at(3, 3) -
                        at(0, 1) * at(1, 0) * at(2, 2) * at(3, 3) + at(0, 0) * at(1, 1) * at(2, 2) * at(3, 3);
            } else {
                throw std::logic_error("Determinant calculation is only defined for 4x4 matrices.");
            }
        }

        Matrix inverse() const {
            if constexpr (N == 4 && M == 4) {
                T det = determinant();
                if (std::abs(det) < 1e-9) {
                    throw std::runtime_error("Matrix is singular and cannot be inverted.");
                }
                T invDet = 1 / det;

                Matrix inv; // Initialize the inverse Matrix

                // First row of cofactors, already computed in the example you provided
                inv.at(0, 0) = (at(1, 1) * (at(2, 2) * at(3, 3) - at(2, 3) * at(3, 2)) -
                                at(1, 2) * (at(2, 1) * at(3, 3) - at(2, 3) * at(3, 1)) +
                                at(1, 3) * (at(2, 1) * at(3, 2) - at(2, 2) * at(3, 1))) * invDet;
                inv.at(0, 1) = -(at(0, 1) * (at(2, 2) * at(3, 3) - at(2, 3) * at(3, 2)) -
                                 at(0, 2) * (at(2, 1) * at(3, 3) - at(2, 3) * at(3, 1)) +
                                 at(0, 3) * (at(2, 1) * at(3, 2) - at(2, 2) * at(3, 1))) * invDet;
                inv.at(0, 2) = (at(0, 1) * (at(1, 2) * at(3, 3) - at(1, 3) * at(3, 2)) -
                                at(0, 2) * (at(1, 1) * at(3, 3) - at(1, 3) * at(3, 1)) +
                                at(0, 3) * (at(1, 1) * at(3, 2) - at(1, 2) * at(3, 1))) * invDet;
                inv.at(0, 3) = -(at(0, 1) * (at(1, 2) * at(2, 3) - at(1, 3) * at(2, 2)) -
                                 at(0, 2) * (at(1, 1) * at(2, 3) - at(1, 3) * at(2, 1)) +
                                 at(0, 3) * (at(1, 1) * at(2, 2) - at(1, 2) * at(2, 1))) * invDet;

                // Second row of cofactors
                inv.at(1, 0) = -(at(1, 0) * (at(2, 2) * at(3, 3) - at(2, 3) * at(3, 2)) -
                                 at(1, 2) * (at(2, 0) * at(3, 3) - at(2, 3) * at(3, 0)) +
                                 at(1, 3) * (at(2, 0) * at(3, 2) - at(2, 2) * at(3, 0))) * invDet;
                inv.at(1, 1) = (at(0, 0) * (at(2, 2) * at(3, 3) - at(2, 3) * at(3, 2)) -
                                at(0, 2) * (at(2, 0) * at(3, 3) - at(2, 3) * at(3, 0)) +
                                at(0, 3) * (at(2, 0) * at(3, 2) - at(2, 2) * at(3, 0))) * invDet;
                inv.at(1, 2) = -(at(0, 0) * (at(1, 2) * at(3, 3) - at(1, 3) * at(3, 2)) -
                                 at(0, 2) * (at(1, 0) * at(3, 3) - at(1, 3) * at(3, 0)) +
                                 at(0, 3) * (at(1, 0) * at(3, 2) - at(1, 2) * at(3, 0))) * invDet;
                inv.at(1, 3) = (at(0, 0) * (at(1, 2) * at(2, 3) - at(1, 3) * at(2, 2)) -
                                at(0, 2) * (at(1, 0) * at(2, 3) - at(1, 3) * at(2, 0)) +
                                at(0, 3) * (at(1, 0) * at(2, 2) - at(1, 2) * at(2, 0))) * invDet;

                // Third row of cofactors
                inv.at(2, 0) = (at(1, 0) * (at(2, 1) * at(3, 3) - at(2, 3) * at(3, 1)) -
                                at(1, 1) * (at(2, 0) * at(3, 3) - at(2, 3) * at(3, 0)) +
                                at(1, 3) * (at(2, 0) * at(3, 1) - at(2, 1) * at(3, 0))) * invDet;
                inv.at(2, 1) = -(at(0, 0) * (at(2, 1) * at(3, 3) - at(2, 3) * at(3, 1)) -
                                 at(0, 1) * (at(2, 0) * at(3, 3) - at(2, 3) * at(3, 0)) +
                                 at(0, 3) * (at(2, 0) * at(3, 1) - at(2, 1) * at(3, 0))) * invDet;
                inv.at(2, 2) = (at(0, 0) * (at(1, 1) * at(3, 3) - at(1, 3) * at(3, 1)) -
                                at(0, 1) * (at(1, 0) * at(3, 3) - at(1, 3) * at(3, 0)) +
                                at(0, 3) * (at(1, 0) * at(3, 1) - at(1, 1) * at(3, 0))) * invDet;
                inv.at(2, 3) = -(at(0, 0) * (at(1, 1) * at(2, 3) - at(1, 3) * at(2, 1)) -
                                 at(0, 1) * (at(1, 0) * at(2, 3) - at(1, 3) * at(2, 0)) +
                                 at(0, 3) * (at(1, 0) * at(2, 1) - at(1, 1) * at(2, 0))) * invDet;

                // Fourth row of cofactors
                inv.at(3, 0) = -(at(1, 0) * (at(2, 1) * at(3, 2) - at(2, 2) * at(3, 1)) -
                                 at(1, 1) * (at(2, 0) * at(3, 2) - at(2, 2) * at(3, 0)) +
                                 at(1, 2) * (at(2, 0) * at(3, 1) - at(2, 1) * at(3, 0))) * invDet;
                inv.at(3, 1) = (at(0, 0) * (at(2, 1) * at(3, 2) - at(2, 2) * at(3, 1)) -
                                at(0, 1) * (at(2, 0) * at(3, 2) - at(2, 2) * at(3, 0)) +
                                at(0, 2) * (at(2, 0) * at(3, 1) - at(2, 1) * at(3, 0))) * invDet;
                inv.at(3, 2) = -(at(0, 0) * (at(1, 1) * at(3, 2) - at(1, 2) * at(3, 1)) -
                                 at(0, 1) * (at(1, 0) * at(3, 2) - at(1, 2) * at(3, 0)) +
                                 at(0, 2) * (at(1, 0) * at(3, 1) - at(1, 1) * at(3, 0))) * invDet;
                inv.at(3, 3) = (at(0, 0) * (at(1, 1) * at(2, 2) - at(1, 2) * at(2, 1)) -
                                at(0, 1) * (at(1, 0) * at(2, 2) - at(1, 2) * at(2, 0)) +
                                at(0, 2) * (at(1, 0) * at(2, 1) - at(1, 1) * at(2, 0))) * invDet;

                return inv;
            } else {
                throw std::logic_error("Inverse calculation only implemented for 4x4 matrices.");
            }
        }

        template<std::size_t P, std::size_t Q>
        void setBlock(std::size_t startRow, std::size_t startCol, const Matrix<P, Q, T>& subMatrix) {
            for (std::size_t i = 0; i < P; ++i) {
                for (std::size_t j = 0; j < Q; ++j) {
                    at(i + startRow, j + startCol) = subMatrix.at(i, j);
                }
            }
        }

        //static Matrix<M, N, T> transpose(const Matrix& input);
        static Matrix<N, M, T> random();
        static Matrix<N, M, T> random(T min, T max);

        // Utility functions
        [[nodiscard]] constexpr std::size_t getRows() const { return N; }
        [[nodiscard]] constexpr std::size_t getCols() const { return M; }

        // IO
        friend std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
            os << "Matrix(" << N << "x" << M << "):" << std::endl;
            for (std::size_t i = 0; i < N; ++i) {
                os << "| ";
                for (std::size_t j = 0; j < M; ++j) {
                    os << std::setw(10) << mat.at(i, j) << " ";
                }
                os << "|" << std::endl;
            }
            return os;
        }
    };
}

#endif // MATRIX_HPP
