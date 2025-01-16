/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef PLANE_HPP
#define PLANE_HPP

#include "ANode.hpp"

namespace RayTracer {
    class Plane : public ANode {
    public:
        Math::Point<3> point; // Un point sur le plan
        Math::Vector<3> normal; // La normale au plan
        std::shared_ptr<IMaterial> material;

        Plane() : point(), normal(), material(nullptr) {}

        Plane(const Math::Point<3>& p, const Math::Vector<3>& n, std::shared_ptr<IMaterial> m)
                : point(p), normal(n), material(std::move(m)) {}

        bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const override {
            Ray transformedRay;
            transformedRay.origin = transform.transformPoint(ray.origin);
            transformedRay.direction = transform.transformVector(ray.direction);

            auto denom = normal.dot(transformedRay.direction);
            if (std::abs(denom) < 1e-6) {
                return false;
            }

            auto t = (point - transformedRay.origin).dot(normal) / denom;
            if (!validT.surrounds(t)) {
                return false;
            }

            intersection.t = t;
            intersection.intersection = transformedRay.at(t);

            Math::Vector<3> outwardNormal = normal;
            intersection.setFaceNormal(transformedRay, outwardNormal);
            intersection.material = material;

            return true;
        }
    };
}

#endif
