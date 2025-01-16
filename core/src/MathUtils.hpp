/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>

namespace Math {
    class Utils {
    public:
        static double degrees_to_radians(double degrees) {
            return degrees * M_PI / 180.0;
        }
    };

}

#endif
