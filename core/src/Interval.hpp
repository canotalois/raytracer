/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include <limits>
#include <algorithm>
#include <cmath>

namespace RayTracer {
    class Interval {
    public:
        double min, max;

        constexpr explicit Interval(double min = -std::numeric_limits<double>::infinity(), double max = std::numeric_limits<double>::infinity()) : min(std::min(min, max)), max(std::max(min, max)) {}

        ~Interval() = default;

        constexpr bool operator==(const Interval &rhs) const {
            return min == rhs.min && max == rhs.max;
        }

        constexpr bool operator!=(const Interval &rhs) const {
            return !(*this == rhs);
        }

        [[nodiscard]] constexpr double size() const {
            return max - min;
        }

        [[nodiscard]] constexpr bool contains(double x) const {
            return min <= x && x <= max;
        }

        [[nodiscard]] constexpr bool surrounds(double x) const {
            return min < x && x < max;
        }

        [[nodiscard]] constexpr Interval intersect(const Interval &other) const {
            return Interval(std::min(max, other.max), std::max(min, other.min));
        }

        [[nodiscard]] constexpr Interval unite(const Interval &other) const {
            return Interval(std::min(min, other.min), std::max(max, other.max));
        }

        [[nodiscard]] constexpr double clamp(double value) const {
            return std::max(min, std::min(value, max));
        }

        static const Interval empty;
        static const Interval universe;
    };

    inline double linear_to_gamma(double linear_component)
    {
        if (linear_component > 0)
            return sqrt(linear_component);

        return 0;
    }
}

#endif
