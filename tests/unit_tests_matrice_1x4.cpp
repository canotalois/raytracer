#include <criterion/criterion.h>
#include "../core/src/Matrix.hpp"

// Test de l'opérateur d'accès 'at'
Test(matrix_operators_1x4, at_operator) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 1.0;
    mat.at(0, 1) = 2.0;
    mat.at(0, 2) = 3.0;
    mat.at(0, 3) = 4.0;
    
    cr_assert_eq(mat.at(0, 0), 1.0, "Error in at operator");
    cr_assert_eq(mat.at(0, 1), 2.0, "Error in at operator");
    cr_assert_eq(mat.at(0, 2), 3.0, "Error in at operator");
    cr_assert_eq(mat.at(0, 3), 4.0, "Error in at operator");
}

// Test de l'opérateur d'égalité
Test(matrix_operators_1x4, equality_operator) {
    Math::Matrix<1, 4, double> mat1, mat2;
    mat1.at(0, 0) = 1.0;
    mat1.at(0, 1) = 2.0;
    mat1.at(0, 2) = 3.0;
    mat1.at(0, 3) = 4.0;
    
    mat2.at(0, 0) = 1.0;
    mat2.at(0, 1) = 2.0;
    mat2.at(0, 2) = 3.0;
    mat2.at(0, 3) = 4.0;
    
    cr_assert(mat1 == mat2, "Error in equality operator");
}

// Test de l'opérateur +=
Test(matrix_operators_1x4, plus_equal_operator) {
    Math::Matrix<1, 4, double> mat1, mat2;
    mat1.at(0, 0) = 1.0;
    mat1.at(0, 1) = 2.0;
    mat1.at(0, 2) = 3.0;
    mat1.at(0, 3) = 4.0;
    
    mat2.at(0, 0) = 2.0;
    mat2.at(0, 1) = 3.0;
    mat2.at(0, 2) = 4.0;
    mat2.at(0, 3) = 5.0;
    
    mat1 += mat2;
    
    cr_assert_eq(mat1.at(0, 0), 3.0, "Error in += operator");
    cr_assert_eq(mat1.at(0, 1), 5.0, "Error in += operator");
    cr_assert_eq(mat1.at(0, 2), 7.0, "Error in += operator");
    cr_assert_eq(mat1.at(0, 3), 9.0, "Error in += operator");
}

// Test de l'opérateur -=
Test(matrix_operators_1x4, minus_equal_operator) {
    Math::Matrix<1, 4, double> mat1, mat2;
    mat1.at(0, 0) = 5.0;
    mat1.at(0, 1) = 4.0;
    mat1.at(0, 2) = 3.0;
    mat1.at(0, 3) = 2.0;
    
    mat2.at(0, 0) = 2.0;
    mat2.at(0, 1) = 1.0;
    mat2.at(0, 2) = 1.0;
    mat2.at(0, 3) = 0.0;
    
    mat1 -= mat2;
    
    cr_assert_eq(mat1.at(0, 0), 3.0, "Error in -= operator");
    cr_assert_eq(mat1.at(0, 1), 3.0, "Error in -= operator");
    cr_assert_eq(mat1.at(0, 2), 2.0, "Error in -= operator");
    cr_assert_eq(mat1.at(0, 3), 2.0, "Error in -= operator");
}

// Test de l'opérateur *= (scalaire)
Test(matrix_operators_1x4, scalar_multiplication_equal_operator) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 2.0;
    mat.at(0, 1) = 3.0;
    mat.at(0, 2) = 4.0;
    mat.at(0, 3) = 5.0;
    
    mat *= 2.0;
    
    cr_assert_eq(mat.at(0, 0), 4.0, "Error in *= (scalar) operator");
    cr_assert_eq(mat.at(0, 1), 6.0, "Error in *= (scalar) operator");
    cr_assert_eq(mat.at(0, 2), 8.0, "Error in *= (scalar) operator");
    cr_assert_eq(mat.at(0, 3), 10.0, "Error in *= (scalar) operator");
}

// Test de l'opérateur /= (scalaire)
Test(matrix_operators_1x4, scalar_division_equal_operator) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 6.0;
    mat.at(0, 1) = 8.0;
    mat.at(0, 2) = 10.0;
    mat.at(0, 3) = 12.0;
    
    mat /= 2.0;
    
    cr_assert_eq(mat.at(0, 0), 3.0, "Error in /= (scalar) operator");
    cr_assert_eq(mat.at(0, 1), 4.0, "Error in /= (scalar) operator");
    cr_assert_eq(mat.at(0, 2), 5.0, "Error in /= (scalar) operator");
    cr_assert_eq(mat.at(0, 3), 6.0, "Error in /= (scalar) operator");
}

// Test de l'opérateur *= (matrice)
Test(matrix_operators_1x4, matrix_multiplication_equal_operator) {
    Math::Matrix<1, 4, double> mat1, mat2;
    mat1.at(0, 0) = 2.0;
    mat1.at(0, 1) = 3.0;
    mat1.at(0, 2) = 4.0;
    mat1.at(0, 3) = 5.0;
    
    mat2.at(0, 0) = 2.0;
    mat2.at(0, 1) = 2.0;
    mat2.at(0, 2) = 2.0;
    mat2.at(0, 3) = 2.0;
    
    mat1 *= mat2;
    
    cr_assert_eq(mat1.at(0, 0), 20.0, "Error in *= (matrix) operator");
}

// Test de l'opérateur unaire -
Test(matrix_operators_1x4, unary_minus_operator) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 3.0;
    mat.at(0, 1) = 4.0;
    mat.at(0, 2) = 5.0;
    mat.at(0, 3) = 6.0;
    
    Math::Matrix<1, 4, double> result = -mat;
    
    cr_assert_eq(result.at(0, 0), -3.0, "Error in unary minus operator");
    cr_assert_eq(result.at(0, 1), -4.0, "Error in unary minus operator");
    cr_assert_eq(result.at(0, 2), -5.0, "Error in unary minus operator");
    cr_assert_eq(result.at(0, 3), -6.0, "Error in unary minus operator");
}

// Test de l'opérateur binaire +
Test(matrix_operators_1x4, binary_plus_operator) {
    Math::Matrix<1, 4, double> mat1, mat2;
    mat1.at(0, 0) = 1.0;
    mat1.at(0, 1) = 2.0;
    mat1.at(0, 2) = 3.0;
    mat1.at(0, 3) = 4.0;
    
    mat2.at(0, 0) = 5.0;
    mat2.at(0, 1) = 6.0;
    mat2.at(0, 2) = 7.0;
    mat2.at(0, 3) = 8.0;
    
    Math::Matrix<1, 4, double> result = mat1 + mat2;
    
    cr_assert_eq(result.at(0, 0), 6.0, "Error in binary plus operator");
    cr_assert_eq(result.at(0, 1), 8.0, "Error in binary plus operator");
    cr_assert_eq(result.at(0, 2), 10.0, "Error in binary plus operator");
    cr_assert_eq(result.at(0, 3), 12.0, "Error in binary plus operator");
}

// Test de l'opérateur binaire -
Test(matrix_operators_1x4, binary_minus_operator) {
    Math::Matrix<1, 4, double> mat1, mat2;
    mat1.at(0, 0) = 5.0;
    mat1.at(0, 1) = 6.0;
    mat1.at(0, 2) = 7.0;
    mat1.at(0, 3) = 8.0;
    
    mat2.at(0, 0) = 1.0;
    mat2.at(0, 1) = 2.0;
    mat2.at(0, 2) = 3.0;
    mat2.at(0, 3) = 4.0;
    
    Math::Matrix<1, 4, double> result = mat1 - mat2;
    
    cr_assert_eq(result.at(0, 0), 4.0, "Error in binary minus operator");
    cr_assert_eq(result.at(0, 1), 4.0, "Error in binary minus operator");
    cr_assert_eq(result.at(0, 2), 4.0, "Error in binary minus operator");
    cr_assert_eq(result.at(0, 3), 4.0, "Error in binary minus operator");
}

// Test de l'opérateur * (scalaire)
Test(matrix_operators_1x4, scalar_multiplication_operator) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 2.0;
    mat.at(0, 1) = 3.0;
    mat.at(0, 2) = 4.0;
    mat.at(0, 3) = 5.0;
    
    Math::Matrix<1, 4, double> result = mat * 3.0;
    
    cr_assert_eq(result.at(0, 0), 6.0, "Error in scalar multiplication operator");
    cr_assert_eq(result.at(0, 1), 9.0, "Error in scalar multiplication operator");
    cr_assert_eq(result.at(0, 2), 12.0, "Error in scalar multiplication operator");
    cr_assert_eq(result.at(0, 3), 15.0, "Error in scalar multiplication operator");
}

// Test de l'opérateur / (scalaire)
Test(matrix_operators_1x4, scalar_division_operator) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 6.0;
    mat.at(0, 1) = 9.0;
    mat.at(0, 2) = 12.0;
    mat.at(0, 3) = 15.0;
    
    Math::Matrix<1, 4, double> result = mat / 3.0;
    
    cr_assert_eq(result.at(0, 0), 2.0, "Error in scalar division operator");
    cr_assert_eq(result.at(0, 1), 3.0, "Error in scalar division operator");
    cr_assert_eq(result.at(0, 2), 4.0, "Error in scalar division operator");
    cr_assert_eq(result.at(0, 3), 5.0, "Error in scalar division operator");
}

// Test de l'opérateur + (scalaire)
Test(matrix_operators_1x4, scalar_plus_operator) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 2.0;
    mat.at(0, 1) = 3.0;
    mat.at(0, 2) = 4.0;
    mat.at(0, 3) = 5.0;
    
    Math::Matrix<1, 4, double> result = mat + 3.0;
    
    cr_assert_eq(result.at(0, 0), 5.0, "Error in scalar plus operator");
    cr_assert_eq(result.at(0, 1), 6.0, "Error in scalar plus operator");
    cr_assert_eq(result.at(0, 2), 7.0, "Error in scalar plus operator");
    cr_assert_eq(result.at(0, 3), 8.0, "Error in scalar plus operator");
}

// Test de l'opérateur - (scalaire)
Test(matrix_operators_1x4, scalar_minus_operator) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 5.0;
    mat.at(0, 1) = 6.0;
    mat.at(0, 2) = 7.0;
    mat.at(0, 3) = 8.0;
    
    Math::Matrix<1, 4, double> result = mat - 3.0;
    
    cr_assert_eq(result.at(0, 0), 2.0, "Error in scalar minus operator");
    cr_assert_eq(result.at(0, 1), 3.0, "Error in scalar minus operator");
    cr_assert_eq(result.at(0, 2), 4.0, "Error in scalar minus operator");
    cr_assert_eq(result.at(0, 3), 5.0, "Error in scalar minus operator");
}

// Test de l'opérateur /= (division par zéro)
Test(matrix_operators_1x4, scalar_division_equal_operator_divide_by_zero) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 6.0;
    mat.at(0, 1) = 9.0;
    mat.at(0, 2) = 12.0;
    mat.at(0, 3) = 15.0;
    
    cr_assert_throw(mat /= 0.0, std::runtime_error, "Error in /= (scalar) operator: Divide by zero");
}

// Test de l'opérateur / (scalaire) (division par zéro)
Test(matrix_operators_1x4, scalar_division_operator_divide_by_zero) {
    Math::Matrix<1, 4, double> mat;
    mat.at(0, 0) = 6.0;
    mat.at(0, 1) = 9.0;
    mat.at(0, 2) = 12.0;
    mat.at(0, 3) = 15.0;
    
    size_t i = 0;
    cr_assert_throw(mat / i, std::runtime_error, "Error in scalar division operator: Divide by zero");
}

// Test de l'opérateur / (matrice) (division par zéro)
Test(matrix_operators_1x4, matrix_division_equal_operator_divide_by_zero) {
    Math::Matrix<1, 4, double> mat1, mat2;
    mat1.at(0, 0) = 2.0;
    mat1.at(0, 1) = 4.0;
    mat1.at(0, 2) = 6.0;
    mat1.at(0, 3) = 8.0;
    
    // mat2.at(0, 0) = 0.0; // Diviseur zéro
    
    size_t i = 0;
    cr_assert_throw(mat1 /= i, std::runtime_error, "Error in /= (matrix) operator: Divide by zero");
}
