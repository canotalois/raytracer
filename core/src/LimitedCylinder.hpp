/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef LIMITED_CYLINDER_H
#define LIMITED_CYLINDER_H

#include "ANode.hpp"

namespace RayTracer {
    class LimitedCylinder : public ANode {
    public:
        LimitedCylinder(const Math::Point<3>& base, double radius, double height, std::shared_ptr<IMaterial> mat)
                : base(base), radius(radius), height(height), mat(std::move(mat)), radiusSquared(radius * radius) {}

        bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const override {
            Ray transformedRay;
            transformedRay.origin = transform.transformPoint(ray.origin);
            transformedRay.direction = transform.transformVector(ray.direction);

            Math::Vector<3> oc = Math::Vector<3>(transformedRay.origin - base);
            double a = transformedRay.direction.at(0,0) * transformedRay.direction.at(0,0) + transformedRay.direction.at(1,0) * transformedRay.direction.at(1,0);
            double b = 2.0 * (oc.at(0,0) * transformedRay.direction.at(0,0) + oc.at(1,0) * transformedRay.direction.at(1,0));
            double c = oc.at(0,0) * oc.at(0,0) + oc.at(1,0) * oc.at(1,0) - radiusSquared;
            double discriminant = b * b - 4 * a * c;

            double t_min = std::numeric_limits<double>::infinity();
            bool hit_something = false;

            double root1 = -1, root2 = -1;

            if (discriminant >= 0) {
                double sqrtDiscriminant = std::sqrt(discriminant);
                root1 = (-b - sqrtDiscriminant) / (2 * a);
                root2 = (-b + sqrtDiscriminant) / (2 * a);
                double z1 = (transformedRay.at(root1).at(2,0) - base.at(2,0));
                double z2 = (transformedRay.at(root2).at(2,0) - base.at(2,0));

                if (validT.surrounds(root1) && z1 >= 0 && z1 <= height) {
                    t_min = root1;
                    hit_something = true;
                }
                if (validT.surrounds(root2) && z2 >= 0 && z2 <= height && root2 < t_min) {
                    t_min = root2;
                    hit_something = true;
                }
            }

            // Intégration des disques de fermeture
            double t_cap;
            if (transformedRay.direction.at(2,0) != 0) {  // Éviter la division par zéro
                // Disque supérieur
                t_cap = (height - (transformedRay.origin.at(2,0) - base.at(2,0))) / transformedRay.direction.at(2,0);
                if (validT.surrounds(t_cap) && Math::Vector(transformedRay.at(t_cap) - Math::Point<3>(
                        {base.at(0, 0), base.at(1, 0), base.at(2, 0) + height})).length() <= radius) {
                    if (t_cap < t_min) {
                        t_min = t_cap;
                        hit_something = true;
                    }
                }

                // Disque inférieur
                t_cap = ((- (transformedRay.origin.at(2,0) - base.at(2,0))) / transformedRay.direction.at(2, 0));
                if (validT.surrounds(t_cap) && Math::Vector(transformedRay.at(t_cap) - base).length() <= radius) {
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
                    Math::Vector<3> outwardNormal = Math::Vector<3>({intersection.intersection.at(0, 0) - base.at(0, 0), intersection.intersection.at(1, 0) - base.at(1, 0), 0}).normalize();
                    intersection.setFaceNormal(transformedRay, outwardNormal);
                } else { // Normale pour les disques
                    intersection.setFaceNormal(transformedRay, Math::Vector<3>({0, 0, static_cast<double>(t_cap == (height - (transformedRay.origin.at(2,0) - base.at(2,0))) / transformedRay.direction.at(2,0) ? 1 : -1)}));
                }
                intersection.material = mat;
                return true;
            }

            return false;
        }

    private:
        Math::Point<3> base;
        double radius;
        double height;
        std::shared_ptr<IMaterial> mat;
        double radiusSquared;
    };
}

#endif