#include "../core/src/GeometryBase.hpp"
#include <criterion/criterion.h>
#include "../core/src/MathUtils.hpp"
#include "../core/src/Color.hpp"
#include "../core/src/Intersection.hpp"

// Test des opérateurs += et -=
Test(geometry_base, plus_minus_equal_operators) {
    Math::GeometryBase<3, double> geom1({1.0, 2.0, 3.0});
    Math::GeometryBase<3, double> geom2({4.0, 5.0, 6.0});
    
    geom1 += geom2;
    cr_assert_float_eq(geom1.at(0, 0), 5.0, 1e-6, "Error in += operator");
    cr_assert_float_eq(geom1.at(1, 0), 7.0, 1e-6, "Error in += operator");
    cr_assert_float_eq(geom1.at(2, 0), 9.0, 1e-6, "Error in += operator");
    
    geom1 -= geom2;
    cr_assert_float_eq(geom1.at(0, 0), 1.0, 1e-6, "Error in -= operator");
    cr_assert_float_eq(geom1.at(1, 0), 2.0, 1e-6, "Error in -= operator");
    cr_assert_float_eq(geom1.at(2, 0), 3.0, 1e-6, "Error in -= operator");
}

// Test de l'opérateur +
Test(geometry_base, plus_operator) {
    Math::GeometryBase<3, double> geom1({1.0, 2.0, 3.0});
    Math::GeometryBase<3, double> geom2({4.0, 5.0, 6.0});
    
    Math::GeometryBase<3, double> result = geom1 + geom2;
    cr_assert_float_eq(result.at(0, 0), 5.0, 1e-6, "Error in + operator");
    cr_assert_float_eq(result.at(1, 0), 7.0, 1e-6, "Error in + operator");
    cr_assert_float_eq(result.at(2, 0), 9.0, 1e-6, "Error in + operator");
}

// Test de l'opérateur -
Test(geometry_base, minus_operator) {
    Math::GeometryBase<3, double> geom1({4.0, 5.0, 6.0});
    Math::GeometryBase<3, double> geom2({1.0, 2.0, 3.0});
    
    Math::GeometryBase<3, double> result = geom1 - geom2;
    cr_assert_float_eq(result.at(0, 0), 3.0, 1e-6, "Error in - operator");
    cr_assert_float_eq(result.at(1, 0), 3.0, 1e-6, "Error in - operator");
    cr_assert_float_eq(result.at(2, 0), 3.0, 1e-6, "Error in - operator");
}

// Test de l'opérateur dot
Test(geometry_base, dot_operator) {
    Math::GeometryBase<3, double> geom1({1.0, 2.0, 3.0});
    Math::GeometryBase<3, double> geom2({4.0, 5.0, 6.0});
    
    double result = geom1.dot(geom2);
    cr_assert_float_eq(result, 32.0, 1e-6, "Error in dot operator");
}

// Test de la méthode degrees_to_radians
Test(utils, degrees_to_radians) {
    double degrees = 90.0;
    double radians = Math::Utils::degrees_to_radians(degrees);
    double expectedRadians = M_PI / 2.0;
    
    cr_assert_float_eq(radians, expectedRadians, 1e-6, "Error in degrees_to_radians method");
}

// Test de la méthode degrees_to_radians avec 0 degrés
Test(utils, degrees_to_radians_zero_degrees) {
    double degrees = 0.0;
    double radians = Math::Utils::degrees_to_radians(degrees);
    double expectedRadians = 0.0;
    
    cr_assert_float_eq(radians, expectedRadians, 1e-6, "Error in degrees_to_radians method with 0 degrees");
}

// Test de la création d'une couleur avec des valeurs spécifiques
Test(color, creation) {
    RayTracer::Color color(0.5, 0.3, 0.2);
    
    cr_assert_eq(color.r(), 0.5, "Error in creating color: Red component");
    cr_assert_eq(color.g(), 0.3, "Error in creating color: Green component");
    cr_assert_eq(color.b(), 0.2, "Error in creating color: Blue component");
}

// Test de la multiplication d'une couleur par un scalaire
Test(color, scalar_multiplication) {
    RayTracer::Color color(0.5, 0.3, 0.2);
    double scalar = 2.0;
    RayTracer::Color result = color * scalar;
    
    cr_assert_eq(result.r(), 1.0, "Error in scalar multiplication: Red component");
    cr_assert_eq(result.g(), 0.6, "Error in scalar multiplication: Green component");
    cr_assert_eq(result.b(), 0.4, "Error in scalar multiplication: Blue component");
}

// Test de l'addition de deux couleurs
Test(color, addition) {
    RayTracer::Color color1(0.5, 0.3, 0.2);
    RayTracer::Color color2(0.2, 0.4, 0.6);
    RayTracer::Color result = color1 + color2;
    
    cr_assert_eq(result.r(), 0.7, "Error in addition: Red component");
    cr_assert_eq(result.g(), 0.7, "Error in addition: Green component");
    cr_assert_eq(result.b(), 0.8, "Error in addition: Blue component");
}
