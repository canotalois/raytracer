/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef INODE_HPP
#define INODE_HPP

#include "Transformation.hpp"
#include "Intersection.hpp"
#include "Interval.hpp"
#include "Ray.hpp"

namespace RayTracer {
    class INode {
    public:
        virtual ~INode() = default;
        virtual bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const = 0;
        virtual void setTransformation(const Transformation& t) = 0;
        [[nodiscard]] virtual const Transformation& getTransformation() const = 0;
    };
}

#endif
