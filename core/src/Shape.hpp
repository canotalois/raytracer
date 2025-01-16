/*
 * ᗧ···ᗣ···ᗣ··
*/

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "INode.hpp"

namespace RayTracer {
    class Shape: public INode {
    public:
        ~Shape() override = 0;

        void setTransformation(const Transformation& t) override {
            transform = t;
        }

        [[nodiscard]] const Transformation& getTransformation() const override {
            return transform;
        }

    private:
        Transformation transform;
    };
}

#endif
