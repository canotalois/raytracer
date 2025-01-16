// #include <criterion/criterion.h>
// #include "../core/src/Vector.hpp"
// #include "../core/src/Matrix.hpp"
// #include <cmath>

// // Test de l'opérateur +=
// Test(vector, plus_equal_operator) {
//     Math::Vector<3, double> vec1;
//     vec1.at(0) = 1.0;
//     vec1.at(1) = 2.0;
//     vec1.at(2) = 3.0;
//     Math::Vector<3, double> vec2;
//     vec2.at(0) = 3.0;
//     vec2.at(1) = 2.0;
//     vec2.at(2) = 1.0;
    
//     vec1 += vec2;
    
//     for (size_t i = 0; i < 3; ++i) {
//         cr_assert_float_eq(vec1.at(i), 4.0, 1e-6, "Error in += operator");
//     }
// }

// // Test de l'opérateur -=
// Test(vector, minus_equal_operator) {
//     Math::Vector<3, double> vec1;
//     vec1.at(0) = 1.0;
//     vec1.at(1) = 2.0;
//     vec1.at(2) = 3.0;
    
//     Math::Vector<3, double> vec2;
//     vec2.at(0) = 3.0;
//     vec2.at(1) = 2.0;
//     vec2.at(2) = 1.0;
    
//     vec1 -= vec2;
    
//     for (size_t i = 0; i < 3; ++i) {
//         cr_assert_float_eq(vec1.at(i), -2.0, 1e-6, "Error in -= operator");
//     }
// }

// // Test de l'opérateur +
// Test(vector, binary_plus_operator) {
//     Math::Vector<3, double> vec1;
//     vec1.at(0) = 1.0;
//     vec1.at(1) = 2.0;
//     vec1.at(2) = 3.0;
    
//     Math::Vector<3, double> vec2;
//     vec2.at(0) = 3.0;
//     vec2.at(1) = 2.0;
//     vec2.at(2) = 1.0;
    
//     Math::Vector<3, double> result = vec1 + vec2;
    
//     for (size_t i = 0; i < 3; ++i) {
//         cr_assert_float_eq(result.at(i), 4.0, 1e-6, "Error in binary plus operator");
//     }
// }

// // Test de l'opérateur -
// Test(vector, binary_minus_operator) {
//     Math::Vector<3, double> vec1;
//     vec1.at(0) = 1.0;
//     vec1.at(1) = 2.0;
//     vec1.at(2) = 3.0;
    
//     Math::Vector<3, double> vec2;
//     vec2.at(0) = 3.0;
//     vec2.at(1) = 2.0;
//     vec2.at(2) = 1.0;
    
//     Math::Vector<3, double> result = vec1 - vec2;
    
//     for (size_t i = 0; i < 3; ++i) {
//         cr_assert_float_eq(result.at(i), -2.0, 1e-6, "Error in binary minus operator");
//     }
// }

// // Test de l'opérateur *
// Test(vector, scalar_multiplication_operator) {
//     Math::Vector<3, double> vec;
//     vec.at(0) = 1.0;
//     vec.at(1) = 2.0;
//     vec.at(2) = 3.0;
    
//     double scalar = 2.0;
    
//     Math::Vector<3, double> result = vec * scalar;
    
//     for (size_t i = 0; i < 3; ++i) {
//         cr_assert_float_eq(result.at(i), 2.0 * (i + 1), 1e-6, "Error in scalar multiplication operator");
//     }
// }

// // Test de l'opérateur /
// Test(vector, scalar_division_operator) {
//     Math::Vector<3, double> vec;
//     vec.at(0) = 2.0;
//     vec.at(1) = 4.0;
//     vec.at(2) = 6.0;
    
//     double scalar = 2.0;
    
//     Math::Vector<3, double> result = vec / scalar;
    
//     for (size_t i = 0; i < 3; ++i) {
//         cr_assert_float_eq(result.at(i), (i + 1), 1e-6, "Error in scalar division operator");
//     }
// }

// // Test de l'opérateur unaire -
// Test(vector, unary_minus_operator) {
//     Math::Vector<3, double> vec;
//     vec.at(0) = 1.0;
//     vec.at(1) = -2.0;
//     vec.at(2) = 3.0;
    
//     Math::Vector<3, double> result = -vec;
    
//     for (size_t i = 0; i < 3; ++i) {
//         cr_assert_float_eq(result.at(i), -vec.at(i), 1e-6, "Error in unary minus operator");
//     }
// }

// // Test de l'opérateur []
// Test(vector, subscript_operator) {
//     Math::Vector<3, double> vec;
//     vec.at(0) = 1.0;
//     vec.at(1) = 2.0;
//     vec.at(2) = 3.0;
    
//     for (size_t i = 0; i < 3; ++i) {
//         cr_assert_float_eq(vec.at(i), (i + 1), 1e-6, "Error in subscript operator");
//     }
// }
