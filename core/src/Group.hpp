/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef GROUP_HPP
#define GROUP_HPP

#include "ANode.hpp"

namespace RayTracer {
    class Group : public ANode {
    public:

        void addNode(const std::shared_ptr<INode>& node) {
            children.push_back(node);
        }

        bool hit(const Ray& ray, const Interval& validT, Intersection& intersection) const override {
            bool hitAnything = false;
            double closestT = validT.max;
            Intersection tempIntersection;
            Ray transformedRay;
            transformedRay.origin = transform.transformPoint(ray.origin);
            transformedRay.direction = transform.transformVector(ray.direction);

            for (const auto& child : children) {
                if (child->hit(transformedRay, Interval(validT.min, closestT), tempIntersection) && tempIntersection.t < closestT) {
                    hitAnything = true;
                    closestT = tempIntersection.t;
                    intersection = tempIntersection;
                }
            }

            return hitAnything;
        }

    private:
        std::vector<std::shared_ptr<INode>> children;
    };
}

#endif
