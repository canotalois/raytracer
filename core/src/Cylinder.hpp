/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "ANode.hpp"

namespace RayTracer {
    class Cylinder : public ANode {
    public:
        Cylinder(const Math::Point<3>& base, double radius, std::shared_ptr<IMaterial> mat)
                : base(base), radius(radius), mat(std::move(mat)), radiusSquared(radius * radius) {}

        bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const override {
            Ray transformedRay;
            transformedRay.origin = transform.transformPoint(ray.origin);
            transformedRay.direction = transform.transformVector(ray.direction);

            Math::Vector<3> oc = Math::Vector<3>(transformedRay.origin - base);
            double a = transformedRay.direction.at(0,0) * transformedRay.direction.at(0,0) + transformedRay.direction.at(1,0) * transformedRay.direction.at(1,0);
            double b = 2.0 * (oc.at(0,0) * transformedRay.direction.at(0,0) + oc.at(1,0) * transformedRay.direction.at(1,0));
            double c = oc.at(0,0) * oc.at(0,0) + oc.at(1,0) * oc.at(1,0) - radiusSquared;
            double discriminant = b * b - 4 * a * c;

            if (discriminant < 0) {
                return false;
            }

            double sqrtDiscriminant = std::sqrt(discriminant);
            double root1 = (-b - sqrtDiscriminant) / (2 * a);
            double root2 = (-b + sqrtDiscriminant) / (2 * a);

            double t_min = std::numeric_limits<double>::infinity();
            bool hit_something = false;

            if (validT.surrounds(root1)) {
                t_min = root1;
                hit_something = true;
            }
            if (validT.surrounds(root2) && root2 < t_min) {
                t_min = root2;
                hit_something = true;
            }

            if (hit_something) {
                intersection.t = t_min;
                intersection.intersection = transformedRay.at(t_min);
                Math::Vector<3> outwardNormal = Math::Vector<3>({
                                                                        intersection.intersection.at(0, 0) - base.at(0, 0),
                                                                        intersection.intersection.at(1, 0) - base.at(1, 0),
                                                                        0
                                                                }).normalize();
                intersection.setFaceNormal(transformedRay, outwardNormal);
                intersection.material = mat;
                return true;
            }

            return false;
        }

    private:
        Math::Point<3> base;
        double radius;
        std::shared_ptr<IMaterial> mat;
        double radiusSquared;
    };
}

#endif
