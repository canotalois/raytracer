/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "ANode.hpp"

namespace RayTracer {
    class Sphere : public ANode {
    public:
        Math::Point<3> center;
        std::shared_ptr<IMaterial> material;
        double radius;

        Sphere() : center(), radius(1.0) {};

        Sphere(const Math::Point<3>& c, double r, std::shared_ptr<IMaterial> m)
        : center(c), radius(r), material(std::move(m)) {};

        bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const override {
            Ray transformedRay;
            transformedRay.origin = transform.transformPoint(ray.origin);
            transformedRay.direction = transform.transformVector(ray.direction);

            const auto oc = center - transformedRay.origin;
            const auto a = transformedRay.direction.dot(transformedRay.direction);
            const auto h = oc.dot(transformedRay.direction);
            const auto c = oc.dot(oc) - radius * radius;

            const auto discriminant = h * h - a * c;

            if (discriminant < 0) {
                return false;
            }

            const auto sqrtDiscriminant = std::sqrt(discriminant);

            double root = (h - sqrtDiscriminant) / a;

            if (!validT.surrounds(root)) {
                root = (h + sqrtDiscriminant) / a;
                if (!validT.surrounds(root))
                    return false;
            }

            intersection.t = root;
            intersection.intersection = transformedRay.at(root);

            Math::Vector<3> outwardNormal = Math::Vector<3>((intersection.intersection - center) / radius);
            intersection.setFaceNormal(transformedRay, outwardNormal);
            intersection.material = material;

            return true;
        }
    };
}

#endif
