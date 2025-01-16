/*
 * ᗧ···ᗣ···ᗣ··
*/

#include "Matrix.hpp"

namespace Math {

    // Constructor from initializer list
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T>::Matrix(std::initializer_list<T> init) {
        if (init.size() != N * M) {
            throw std::length_error("Initializer list size does not match matrix dimensions.");
        }
        std::copy(init.begin(), init.end(), data.begin());
    }

    // Access elements
    template<std::size_t N, std::size_t M, typename T>
    T& Matrix<N, M, T>::at(std::size_t row, std::size_t col) {
        if (row >= N || col >= M)
            throw std::out_of_range("Index out of range");
        return data[row * M + col];
    }

    template<std::size_t N, std::size_t M, typename T>
    const T& Matrix<N, M, T>::at(std::size_t row, std::size_t col) const {
        if (row >= N || col >= M)
            throw std::out_of_range("Index out of range");
        return data[row * M + col];
    }

    // Equality operator
    template<std::size_t N, std::size_t M, typename T>
    bool Matrix<N, M, T>::operator==(const Matrix<N, M, T>& other) const {
        return data == other.data;
    }

    // Addition assignment operator
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix& rhs) {
        std::transform(
                       this->data.begin(), this->data.end(),
                       rhs.data.begin(), this->data.begin(),
                       std::plus<T>());
        return *this;
    }

    // Subtraction assignment operator
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix& rhs) {
        std::transform(
                       this->data.begin(), this->data.end(),
                       rhs.data.begin(), this->data.begin(),
                       std::minus<T>());
        return *this;
    }

    // Scalar multiplication assignment
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const T& scalar) {
        std::transform(
                       this->data.begin(), this->data.end(),
                       this->data.begin(),
                       [&scalar](const T& element) { return element * scalar; });
        return *this;
    }

    // Scalar division assignment
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T>& Matrix<N, M, T>::operator/=(const T& scalar) {
        if (scalar == T(0)) {
            throw std::runtime_error("Division by zero.");
        }
        std::transform(
                       this->data.begin(), this->data.end(),
                       this->data.begin(),
                       [&scalar](const T& element) { return element / scalar; });
        return *this;
    }

    // Matrix multiplication assignment
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T>& Matrix<N, M, T>::operator*=(const Matrix<N, M, T>& rhs) {
        Matrix<N, M, T> result;

        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < M; ++j) {
                T sum = T(0);
                for (std::size_t k = 0; k < M; ++k) {
                    sum += this->data[i * M + k] * rhs.data[k * M + j];
                }
                result.data[i * M + j] = sum;
            }
        }

        this->data = std::move(result.data);
        return *this;
    }

    // Unary minus operator
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::operator-() const {
        Matrix<N, M, T> result;
        std::transform(
                       this->data.begin(), this->data.end(),
                       result.data.begin(),
                       std::negate<T>());
        return result;
    }

    // Addition of matrices
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::operator+(const Matrix<N, M, T>& other) const {
        Matrix<N, M, T> result;
        std::transform(
                this->data.begin(), this->data.end(),
                other.data.begin(),
                result.data.begin(),
                std::plus<T>());
        return result;
    }

    // Subtraction of matrices
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::operator-(const Matrix<N, M, T>& other) const {
        Matrix<N, M, T> result;
        std::transform(
                this->data.begin(), this->data.end(),
                other.data.begin(),
                result.data.begin(),
                std::minus<T>());
        return result;
    }

    // Scalar multiplication
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::operator*(T scalar) const {
        Matrix<N, M, T> result;
        std::transform(
                this->data.begin(), this->data.end(),
                result.data.begin(),
                [&scalar](const T& a) { return a * scalar; });
        return result;
    }

    // Scalar addition
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::operator+(T scalar) const {
        Matrix<N, M, T> result;
        std::transform(
                std::begin(this->data), std::end(this->data),
                result.data.begin(),
                [&scalar](const T& a) { return a + scalar; }
        );
        return result;
    }

    // Scalar subtraction
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::operator-(T scalar) const {
        Matrix<N, M, T> result;
        std::transform(
                std::begin(this->data), std::end(this->data),
                result.data.begin(),
                [&scalar](const T& a) { return a - scalar; }
        );
        return result;
    }

    // Scalar division
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::operator/(T scalar) const {
        if (scalar == T{}) {
            throw std::invalid_argument("Division by zero.");
        }

        Matrix<N, M, T> result;
        std::transform(
                this->data.begin(), this->data.end(),
                result.data.begin(),
                [&scalar](const T& a) { return a / scalar; });
        return result;
    }

    // Matrix transpose
    /*
    template<std::size_t N, std::size_t M, typename T>
    Matrix<M, N, T> Matrix<N, M, T>::transpose() const {
        Matrix<M, N, T> result;

        for (std::size_t i = 0; i < N; ++i) {
            for (std::size_t j = 0; j < M; ++j) {
                result.data[j * N + i] = this->data[i * M + j];
            }
        }

        return result;
    }*/

    // Generate random matrix
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::random() {
        Matrix<N, M, T> result;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<T> dis(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());

        for (std::size_t i = 0; i < N * M; ++i) {
            result.data[i] = dis(gen);
        }

        return result;
    }

    // Generate random matrix with specified bounds
    template<std::size_t N, std::size_t M, typename T>
    Matrix<N, M, T> Matrix<N, M, T>::random(T min, T max) {
        Matrix<N, M, T> result;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<T> dis(min, max);

        for (std::size_t i = 0; i < N * M; ++i) {
            result.data[i] = dis(gen);
        }

        return result;
    }

    // Explicit template instantiation
    template class Matrix<3, 1, double>;
    template class Matrix<1, 3, double>;
    template class Matrix<4, 1, double>;
    template class Matrix<1, 4, double>;
    template class Matrix<3, 3, double>;
    template class Matrix<4, 4, double>;

}
