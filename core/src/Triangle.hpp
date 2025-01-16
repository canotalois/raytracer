/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "ANode.hpp"
#include <memory>
#include <utility>

namespace RayTracer {
    class Triangle : public ANode {
    public:
        Math::Point<3> v0, v1, v2;
        std::shared_ptr<IMaterial> material;

        Triangle(const Math::Point<3>& v0, const Math::Point<3>& v1, const Math::Point<3>& v2, std::shared_ptr<IMaterial> material)
                : v0(v0), v1(v1), v2(v2), material(std::move(material)) {}

        bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const override {
            Ray transformedRay;
            transformedRay.origin = transform.transformPoint(ray.origin);
            transformedRay.direction = transform.transformVector(ray.direction);

            const double EPSILON = 0.0000001;
            Math::Vector<3> edge1 = Math::Vector<3>(v1 - v0);
            Math::Vector<3> edge2 = Math::Vector<3>(v2 - v0);
            Math::Vector<3> h = Math::Vector<3>::cross(ray.direction, edge2);
            double a = edge1.dot(h);

            if (a > -EPSILON && a < EPSILON)
                return false;

            double f = 1.0 / a;
            Math::Vector<3> s = Math::Vector<3> (transformedRay.origin - v0);
            double u = f * (s.dot(h));
            if (u < 0.0 || u > 1.0)
                return false;
            Math::Vector<3> q = Math::Vector<3>::cross(s, edge1);
            double v = f * transformedRay.direction.dot(q);
            if (v < 0.0 || u + v > 1.0)
                return false;

            double t = f * edge2.dot(q);
            if (t > EPSILON && t < validT.max) {
                intersection.t = t;
                intersection.intersection = transformedRay.at(t);
                Math::Vector<3> outwardNormal = Math::Vector<3>::cross(edge1, edge2).normalize();
                intersection.setFaceNormal(transformedRay, outwardNormal);
                intersection.material = material;
                return true;
            }

            return false;
        }
    };
}

#endif
