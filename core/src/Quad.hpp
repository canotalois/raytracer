/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef QUAD_H
#define QUAD_H

#include <utility>

#include "ANode.hpp"

namespace RayTracer {
    class Quad : public ANode {
    public:
        Quad(const Math::Point<3>& Q, const Math::Vector<3> &u, const Math::Vector<3> &v, std::shared_ptr<IMaterial> mat)
                : Q(Q), u(u), v(v), mat(std::move(mat)) {
            normal = Math::Vector<3>::cross(u, v).normalize();
        }


        bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const override {
            Ray transformedRay;
            transformedRay.origin = transform.transformPoint(ray.origin);
            transformedRay.direction = transform.transformVector(ray.direction);

            auto denom = normal.dot(transformedRay.direction);

            if (std::abs(denom) < 1e-6) {
                return false;
            }

            auto t = (Q - transformedRay.origin).dot(normal) / denom;
            if (!validT.surrounds(t)) {
                return false;
            }

            Math::Point<3> P = transformedRay.at(t);
            Math::Vector<3> PQ = Math::Vector<3>(P - Q);

            double uCoord = PQ.dot(u);
            double vCoord = PQ.dot(v);

            if (uCoord < 0 || uCoord > u.dot(u) || vCoord < 0 || vCoord > v.dot(v)) {
                return false;
            }

            intersection.t = t;
            intersection.intersection = P;
            intersection.setFaceNormal(transformedRay, normal);
            intersection.material = mat;

            return true;
        }

    private:
        Math::Point<3> Q;
        Math::Vector<3> u, v;
        std::shared_ptr<IMaterial> mat;
        Math::Vector<3> normal;
    };
}

#endif
