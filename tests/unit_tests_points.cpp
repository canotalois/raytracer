#include <criterion/criterion.h>
#include "../core/src/Point.hpp"

// Test de l'opérateur +=
Test(point, plus_equal_operator) {
    Math::Point<3, double> point1;
    point1.at(0, 0) = 1.0;
    point1.at(1, 0) = 2.0;
    point1.at(2, 0) = 3.0;
    
    Math::Point<3, double> point2;
    point2.at(0, 0) = 3.0;
    point2.at(1, 0) = 2.0;
    point2.at(2, 0) = 1.0;
    
    point1 += point2;
    
    for (size_t i = 0; i < 3; ++i) {
        cr_assert_float_eq(point1.at(i, 0), 4.0, 1e-6, "Error in += operator");
    }
}

// Test de l'opérateur -=
Test(point, minus_equal_operator) {
    Math::Point<3, double> point1;
    point1.at(0, 0) = 1.0;
    point1.at(1, 0) = 2.0;
    point1.at(2, 0) = 3.0;
    
    Math::Point<3, double> point2;
    point2.at(0, 0) = 3.0;
    point2.at(1, 0) = 2.0;
    point2.at(2, 0) = 1.0;
    
    point1 -= point2;
    
    for (size_t i = 0; i < 3; ++i) {
        cr_assert_float_eq(point1.at(i, 0), -2.0, 1e-6, "Error in -= operator");
    }
}

// Test de l'opérateur +
Test(point, binary_plus_operator) {
    Math::Point<3, double> point1;
    point1.at(0, 0) = 1.0;
    point1.at(1, 0) = 2.0;
    point1.at(2, 0) = 3.0;
    
    Math::Point<3, double> point2;
    point2.at(0, 0) = 3.0;
    point2.at(1, 0) = 2.0;
    point2.at(2, 0) = 1.0;
    
    Math::Point<3, double> result = point1 + point2;
    
    for (size_t i = 0; i < 3; ++i) {
        cr_assert_float_eq(result.at(i, 0), 4.0, 1e-6, "Error in binary plus operator");
    }
}

// Test de l'opérateur -
Test(point, binary_minus_operator) {
    Math::Point<3, double> point1;
    point1.at(0, 0) = 1.0;
    point1.at(1, 0) = 2.0;
    point1.at(2, 0) = 3.0;
    
    Math::Point<3, double> point2;
    point2.at(0, 0) = 3.0;
    point2.at(1, 0) = 2.0;
    point2.at(2, 0) = 1.0;
    
    Math::Point<3, double> result = point1 - point2;
    
    for (size_t i = 0; i < 3; ++i) {
        cr_assert_float_eq(result.at(i, 0), -2.0, 1e-6, "Error in binary minus operator");
    }
}

// Test de l'opérateur *
Test(point, scalar_multiplication_operator) {
    Math::Point<3, double> point;
    point.at(0, 0) = 1.0;
    point.at(1, 0) = 2.0;
    point.at(2, 0) = 3.0;
    
    double scalar = 2.0;
    
    Math::Point<3, double> result = point * scalar;
    
    for (size_t i = 0; i < 3; ++i) {
        cr_assert_float_eq(result.at(i, 0), 2.0 * (i + 1), 1e-6, "Error in scalar multiplication operator");
    }
}

// Test de l'opérateur /
Test(point, scalar_division_operator) {
    Math::Point<3, double> point;
    point.at(0, 0) = 2.0;
    point.at(1, 0) = 4.0;
    point.at(2, 0) = 6.0;
    
    double scalar = 2.0;
    
    Math::Point<3, double> result = point / scalar;
    
    for (size_t i = 0; i < 3; ++i) {
        cr_assert_float_eq(result.at(i, 0), (i + 1), 1e-6, "Error in scalar division operator");
    }
}

Test(point, assignment_operator) {
    Math::Point<3, double> point1;
    point1.at(0, 0) = 1.0;
    point1.at(1, 0) = 2.0;
    point1.at(2, 0) = 3.0;
    
    Math::Point<3, double> point2;
    point2 = point1;
    
    for (size_t i = 0; i < 3; ++i) {
        cr_assert_float_eq(point2.at(i, 0), point1.at(i, 0), 1e-6, "Error in assignment operator");
    }
}
