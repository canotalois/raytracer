/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "Point.hpp"
#include "Vector.hpp"
#include "Ray.hpp"

namespace RayTracer {
    class IMaterial;

    class Intersection {
    public:
        Math::Point<3> intersection;
        Math::Vector<3> normal;
        std::shared_ptr<IMaterial> material;
        double t;
        bool frontFace;

        void setFaceNormal(const Ray& r, const Math::Vector<3>& outwardNormal) {
            frontFace = r.direction.dot(outwardNormal) < 0;
            normal = frontFace ? outwardNormal : Math::Vector<3>(-outwardNormal);
        }
    };
}

#endif
