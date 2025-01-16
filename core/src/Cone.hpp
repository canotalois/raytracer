/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef CONE_HPP
#define CONE_HPP

#include "ANode.hpp"
#include <memory>
#include <cmath>
#include <utility>

namespace RayTracer {
    class Cone : public ANode {
    public:
        Cone(const Math::Point<3>& apex, double angle, std::shared_ptr<IMaterial> mat)
                : apex(apex), angle(angle), mat(std::move(mat)) {
            tanSquared = std::tan(angle) * std::tan(angle);
        }

        bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const override {
            Ray transformedRay;
            transformedRay.origin = transform.transformPoint(ray.origin);
            transformedRay.direction = transform.transformVector(ray.direction);

            Math::Vector<3> oc = Math::Vector<3>(transformedRay.origin - apex);
            double a = transformedRay.direction.at(0,0) * transformedRay.direction.at(0,0) + transformedRay.direction.at(1,0) * transformedRay.direction.at(1,0) - tanSquared * transformedRay.direction.at(2,0) * transformedRay.direction.at(2,0);
            double b = 2.0 * (oc.at(0,0) * transformedRay.direction.at(0,0) + oc.at(1,0) * transformedRay.direction.at(1,0) - tanSquared * oc.at(2,0) * transformedRay.direction.at(2,0));
            double c = oc.at(0,0) * oc.at(0,0) + oc.at(1,0) * oc.at(1,0) - tanSquared * oc.at(2,0) * oc.at(2,0);
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
                double z1 = apex.at(2,0) - transformedRay.at(root1).at(2,0);
                if (z1 >= 0) {
                    t_min = root1;
                    hit_something = true;
                }
            }
            if (validT.surrounds(root2) && root2 < t_min) {
                double z2 = apex.at(2,0) - transformedRay.at(root2).at(2,0);
                if (z2 >= 0) {
                    t_min = root2;
                    hit_something = true;
                }
            }

            if (hit_something) {
                intersection.t = t_min;
                intersection.intersection = transformedRay.at(t_min);
                Math::Vector<3> outwardNormal = Math::Vector<3>({
                                                                        intersection.intersection.at(0, 0) - apex.at(0, 0),
                                                                        intersection.intersection.at(1, 0) - apex.at(1, 0),
                                                                        tanSquared * (apex.at(2, 0) - intersection.intersection.at(2, 0))
                                                                }).normalize();
                intersection.setFaceNormal(transformedRay, outwardNormal);
                intersection.material = mat;
                return true;
            }

            return false;
        }

    private:
        Math::Point<3> apex;
        double angle;
        double tanSquared;
        std::shared_ptr<IMaterial> mat;
    };
}

#endif
