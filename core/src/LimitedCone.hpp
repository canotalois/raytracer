/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef LIMITED_CONE_HPP
#define LIMITED_CONE_HPP

#include "ANode.hpp"

namespace RayTracer {
    class LimitedCone : public ANode {
    public:
        LimitedCone(const Math::Point<3>& apex, double radius, double height, std::shared_ptr<IMaterial> mat)
                : apex(apex), radius(radius), height(height), mat(std::move(mat)) {}

        bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const override {
            Ray transformedRay;
            transformedRay.origin = transform.transformPoint(ray.origin);
            transformedRay.direction = transform.transformVector(ray.direction);

            Math::Vector<3> oc = Math::Vector<3>(transformedRay.origin - apex);
            double k = radius / height;
            double k2 = k * k;
            double a = transformedRay.direction.at(0,0) * transformedRay.direction.at(0,0) + transformedRay.direction.at(1,0) * transformedRay.direction.at(1,0) - k2 * transformedRay.direction.at(2,0) * transformedRay.direction.at(2,0);
            double b = 2.0 * (oc.at(0,0) * transformedRay.direction.at(0,0) + oc.at(1,0) * transformedRay.direction.at(1,0) - k2 * oc.at(2,0) * transformedRay.direction.at(2,0));
            double c = oc.at(0,0) * oc.at(0,0) + oc.at(1,0) * oc.at(1,0) - k2 * oc.at(2,0) * oc.at(2,0);
            double discriminant = b * b - 4 * a * c;

            double t_min = std::numeric_limits<double>::infinity();
            bool hit_something = false;

            double root1 = -1, root2 = -1;

            if (discriminant >= 0) {
                double sqrtDiscriminant = std::sqrt(discriminant);
                root1 = (-b - sqrtDiscriminant) / (2 * a);
                root2 = (-b + sqrtDiscriminant) / (2 * a);
                double z1 = apex.at(2,0) - transformedRay.at(root1).at(2,0);
                double z2 = apex.at(2,0) - transformedRay.at(root2).at(2,0);

                if (validT.surrounds(root1) && z1 >= 0 && z1 <= height) {
                    t_min = root1;
                    hit_something = true;
                }
                if (validT.surrounds(root2) && z2 >= 0 && z2 <= height && root2 < t_min) {
                    t_min = root2;
                    hit_something = true;
                }
            }

            // Intersection avec le disque de base
            double t_cap;
            if (transformedRay.direction.at(2,0) != 0) {  // Éviter la division par zéro
                t_cap = (apex.at(2,0) - height - transformedRay.origin.at(2,0)) / transformedRay.direction.at(2,0);
                Math::Point<3> base_center = Math::Point<3>({apex.at(0,0), apex.at(1,0), apex.at(2,0) - height});
                if (validT.surrounds(t_cap) && Math::Vector(transformedRay.at(t_cap) - base_center).length() <= radius) {
                    if (t_cap < t_min) {
                        t_min = t_cap;
                        hit_something = true;
                    }
                }
            }

            if (hit_something) {
                intersection.t = t_min;
                intersection.intersection = transformedRay.at(t_min);
                if (intersection.t == root1 || intersection.t == root2) {
                    Math::Vector<3> outwardNormal = Math::Vector<3>({
                                                                            intersection.intersection.at(0, 0) - apex.at(0, 0),
                                                                            intersection.intersection.at(1, 0) - apex.at(1, 0),
                                                                            height / radius * (apex.at(2, 0) - intersection.intersection.at(2, 0))
                                                                    }).normalize();
                    intersection.setFaceNormal(transformedRay, outwardNormal);
                } else { // Normale pour le disque de base
                    intersection.setFaceNormal(transformedRay, Math::Vector<3>({0, 0, 1}));
                }
                intersection.material = mat;
                return true;
            }

            return false;
        }

    private:
        Math::Point<3> apex;
        double radius;
        double height;
        std::shared_ptr<IMaterial> mat;
    };
}

#endif