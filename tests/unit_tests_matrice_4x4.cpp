#include <criterion/criterion.h>
#include "../core/src/Matrix.hpp"
#include <stdexcept>

// Test de l'opérateur d'accès 'at'
Test(matrix_operators_4x4, at_operator) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 1.0;
    
    cr_assert_eq(mat.at(0, 0), 1.0, "Error in at operator");
}

// Test de l'opérateur d'égalité
Test(matrix_operators_4x4, equality_operator) {
    Math::Matrix<4, 4, double> mat1, mat2;
    mat1.at(0, 0) = 1.0;
    mat2.at(0, 0) = 1.0;
    
    cr_assert(mat1 == mat2, "Error in equality operator");
}

// Test de l'opérateur +=
Test(matrix_operators_4x4, plus_equal_operator) {
    Math::Matrix<4, 4, double> mat1, mat2;
    mat1.at(0, 0) = 1.0;
    mat2.at(0, 0) = 2.0;
    
    mat1 += mat2;
    
    cr_assert_eq(mat1.at(0, 0), 3.0, "Error in += operator");
}

// Test de l'opérateur -=
Test(matrix_operators_4x4, minus_equal_operator) {
    Math::Matrix<4, 4, double> mat1, mat2;
    mat1.at(0, 0) = 3.0;
    mat2.at(0, 0) = 2.0;
    
    mat1 -= mat2;
    
    cr_assert_eq(mat1.at(0, 0), 1.0, "Error in -= operator");
}

// Test de l'opérateur *= (scalaire)
Test(matrix_operators_4x4, scalar_multiplication_equal_operator) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 2.0;
    
    mat *= 3.0;
    
    cr_assert_eq(mat.at(0, 0), 6.0, "Error in *= (scalar) operator");
}

// Test de l'opérateur /= (scalaire)
Test(matrix_operators_4x4, scalar_division_equal_operator) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 6.0;
    
    mat /= 2.0;
    
    cr_assert_eq(mat.at(0, 0), 3.0, "Error in /= (scalar) operator");
}

// Test de l'opérateur *= (matrice)
Test(matrix_operators_4x4, matrix_multiplication_equal_operator) {
    Math::Matrix<4, 4, double> mat1, mat2;
    mat1.at(0, 0) = 2.0;
    mat2.at(0, 0) = 3.0;
    
    mat1 *= mat2;
    
    cr_assert_eq(mat1.at(0, 0), 6.0, "Error in *= (matrix) operator");
}

// Test de l'opérateur unaire -
Test(matrix_operators_4x4, unary_minus_operator) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 3.0;
    
    Math::Matrix<4, 4, double> result = -mat;
    
    cr_assert_eq(result.at(0, 0), -3.0, "Error in unary minus operator");
}

// Test de l'opérateur binaire +
Test(matrix_operators_4x4, binary_plus_operator) {
    Math::Matrix<4, 4, double> mat1, mat2;
    mat1.at(0, 0) = 1.0;
    mat2.at(0, 0) = 2.0;
    
    Math::Matrix<4, 4, double> result = mat1 + mat2;
    
    cr_assert_eq(result.at(0, 0), 3.0, "Error in binary plus operator");
}

// Test de l'opérateur binaire -
Test(matrix_operators_4x4, binary_minus_operator) {
    Math::Matrix<4, 4, double> mat1, mat2;
    mat1.at(0, 0) = 3.0;
    mat2.at(0, 0) = 2.0;
    
    Math::Matrix<4, 4, double> result = mat1 - mat2;
    
    cr_assert_eq(result.at(0, 0), 1.0, "Error in binary minus operator");
}

// Test de l'opérateur * (scalaire)
Test(matrix_operators_4x4, scalar_multiplication_operator) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 2.0;
    
    Math::Matrix<4, 4, double> result = mat * 3.0;
    
    cr_assert_eq(result.at(0, 0), 6.0, "Error in scalar multiplication operator");
}

// Test de l'opérateur / (scalaire)
Test(matrix_operators_4x4, scalar_division_operator) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 6.0;
    
    Math::Matrix<4, 4, double> result = mat / 2.0;
    
    cr_assert_eq(result.at(0, 0), 3.0, "Error in scalar division operator");
}

// Test de l'opérateur + (scalaire)
Test(matrix_operators_4x4, scalar_plus_operator) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 2.0;
    
    Math::Matrix<4, 4, double> result = mat + 3.0;
    
    cr_assert_eq(result.at(0, 0), 5.0, "Error in scalar plus operator");
}

// Test de l'opérateur - (scalaire)
Test(matrix_operators_4x4, scalar_minus_operator) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 5.0;
    
    Math::Matrix<4, 4, double> result = mat - 3.0;
    
    cr_assert_eq(result.at(0, 0), 2.0, "Error in scalar minus operator");
}

// Test de l'opérateur /= (division par zéro)
Test(matrix_operators_4x4, scalar_division_equal_operator_divide_by_zero) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 6.0;
    
    cr_assert_throw(mat /= 0.0, std::runtime_error, "Error in /= (scalar) operator: Divide by zero");
}

// Test de l'opérateur / (scalaire) (division par zéro)
Test(matrix_operators_4x4, scalar_division_operator_divide_by_zero) {
    Math::Matrix<4, 4, double> mat;
    mat.at(0, 0) = 6.0;
    size_t i = 0;
    cr_assert_throw(mat / i, std::runtime_error, "Error in scalar division operator: Divide by zero");
}

// Test de l'opérateur / (matrice) (division par zéro)
Test(matrix_operators_4x4, matrix_division_equal_operator_divide_by_zero) {
    Math::Matrix<4, 4, double> mat1, mat2;
    mat1.at(0, 0) = 2.0;
    // mat2.at(0, 0) = 0.0; // Diviseur zéro
    size_t i = 0;
    
    cr_assert_throw(mat1 /= i, std::runtime_error, "Error in /= (matrix) operator: Divide by zero");
}
